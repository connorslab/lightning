#include "config.h"
#include <assert.h>
#include <common/features.h>
#include <common/setup.h>
#include <common/utils.h>
#include <wire/peer_wire.h>

int main(int argc, char *argv[])
{
	struct feature_set *blake, *legacy;
	struct tlv_init_tlvs *tlvs, *decoded;
	u8 *msg, *global, *features, *empty;
	common_setup(argv[0]);
	blake = feature_set_for_feature(tmpctx, OPT_BLAKE2B);
	legacy = feature_set_for_feature(tmpctx, OPT_STATIC_REMOTEKEY);
	empty = tal_arr(tmpctx, u8, 0);
	assert(feature_is_set(blake->bits[INIT_FEATURE], OPT_BLAKE2B + 1));
	assert(!feature_is_set(blake->bits[INIT_FEATURE], OPT_BLAKE2B));
	assert(feature_is_set(blake->bits[NODE_ANNOUNCE_FEATURE], OPT_BLAKE2B + 1));
	assert(features_unsupported(legacy, blake->bits[INIT_FEATURE], INIT_FEATURE) == -1);
	assert(features_unsupported(blake, empty, INIT_FEATURE) == -1);
	for (size_t i = BOLT11_FEATURE; i <= BOLT12_INVOICE_FEATURE; i++)
		assert(tal_bytelen(blake->bits[i]) == 0);
	assert(OPT_BLAKE2B == 68);
	assert(tal_bytelen(blake->bits[INIT_FEATURE]) == 9);
	assert(!feature_offered(blake->bits[CHANNEL_TYPE_FEATURE], OPT_BLAKE2B));
	assert(!feature_offered(blake->bits[CHANNEL_FEATURE], OPT_BLAKE2B));
	/* A BOLT11 field holds at most 1023 five-bit groups. */
	assert(OPT_BLAKE2B < 1023 * 5);
	tlvs = tlv_init_tlvs_new(tmpctx);
	msg = towire_init(tmpctx, empty, blake->bits[INIT_FEATURE], tlvs);
	assert(fromwire_init(tmpctx, msg, &global, &features, &decoded));
	assert(feature_is_set(features, OPT_BLAKE2B + 1));
	common_shutdown();
	return 0;
}
