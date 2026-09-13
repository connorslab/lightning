#include "config.h"
#include <assert.h>
#include <common/setup.h>
#include <connectd/blake2b_identity.h>
#include <wire/peer_wire.h>

int main(int argc, char *argv[])
{
	struct tlv_init_tlvs *tlvs, *decoded;
	u8 *msg, *global, *features, *empty;
	common_setup(argv[0]);
	assert(!blake2b_identity_matches(NULL, 0));
	assert(!blake2b_identity_matches("", 0));
	assert(!blake2b_identity_matches(BLAKE2B_IDENTITY,
					 sizeof(BLAKE2B_IDENTITY)));
	assert(!blake2b_identity_matches("bitcoin-blake2b/header-v2/2", 25));
	assert(blake2b_identity_matches(BLAKE2B_IDENTITY,
				       sizeof(BLAKE2B_IDENTITY) - 1));
	empty = tal_arr(tmpctx, u8, 0);
	tlvs = tlv_init_tlvs_new(tmpctx);
	msg = towire_init(tmpctx, empty, empty, tlvs);
	assert(fromwire_init(tmpctx, msg, &global, &features, &decoded));
	assert(!blake2b_identity_matches(decoded->blake2b_identity,
					tal_count(decoded->blake2b_identity)));
	tlvs->blake2b_identity = tal_dup_arr(tlvs, u8,
		(const u8 *)BLAKE2B_IDENTITY, sizeof(BLAKE2B_IDENTITY) - 1, 0);
	msg = towire_init(tmpctx, empty, empty, tlvs);
	assert(fromwire_init(tmpctx, msg, &global, &features, &decoded));
	assert(blake2b_identity_matches(decoded->blake2b_identity,
				       tal_count(decoded->blake2b_identity)));
	tal_resize(&msg, tal_count(msg) - 1);
	assert(!fromwire_init(tmpctx, msg, &global, &features, &decoded));
	common_shutdown();
	return 0;
}
