#include "config.h"
#include <assert.h>
#include <common/features.h>
#include <common/setup.h>
#include <common/utils.h>
#include <wire/peer_wire.h>

int main(int argc, char *argv[])
{
	struct feature_set *ours, *legacy;
	struct tlv_init_tlvs *tlvs, *decoded;
	u8 *msg, *global, *features, *empty, *optional;
	common_setup(argv[0]);
	ours = feature_set_for_feature(tmpctx, OPT_BLAKE2B_NETWORK);
	/* Published big-endian wire vector: 04 followed by 530 zero bytes. */
	assert(tal_count(ours->bits[INIT_FEATURE]) == 531);
	assert(ours->bits[INIT_FEATURE][0] == 4);
	for (size_t i = 1; i < 531; i++)
		assert(ours->bits[INIT_FEATURE][i] == 0);
	legacy = feature_set_for_feature(tmpctx, OPT_STATIC_REMOTEKEY);
	empty = tal_arr(tmpctx, u8, 0);
	assert(!feature_offered(empty, OPT_BLAKE2B_NETWORK));
	assert(features_unsupported(legacy, ours->bits[INIT_FEATURE],
				    INIT_FEATURE) == OPT_BLAKE2B_NETWORK);
	assert(features_unsupported(ours, ours->bits[INIT_FEATURE],
				    INIT_FEATURE) == -1);
	for (size_t i = 1; i < NUM_FEATURE_PLACE; i++)
		assert(!feature_offered(ours->bits[i], OPT_BLAKE2B_NETWORK));
	optional = tal_arr(tmpctx, u8, 0);
	set_feature_bit(&optional, OPTIONAL_FEATURE(OPT_BLAKE2B_NETWORK));
	assert(feature_offered(optional, OPT_BLAKE2B_NETWORK));
	tlvs = tlv_init_tlvs_new(tmpctx);
	msg = towire_init(tmpctx, empty, ours->bits[INIT_FEATURE], tlvs);
	assert(fromwire_init(tmpctx, msg, &global, &features, &decoded));
	assert(feature_is_set(features, OPT_BLAKE2B_NETWORK));
	assert(!feature_is_set(features, OPTIONAL_FEATURE(OPT_BLAKE2B_NETWORK)));
	tal_resize(&msg, tal_count(msg) - 1);
	assert(!fromwire_init(tmpctx, msg, &global, &features, &decoded));
	common_shutdown();
	return 0;
}
