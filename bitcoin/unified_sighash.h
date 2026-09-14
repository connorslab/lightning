#ifndef LIGHTNING_BITCOIN_UNIFIED_SIGHASH_H
#define LIGHTNING_BITCOIN_UNIFIED_SIGHASH_H
#include "config.h"
#include <bitcoin/tx.h>

/* Proposal foundation only: callers must supply authenticated spent outputs.
 * No feature advertisement or automatic channel upgrade is enabled here. */
struct unified_sighash_input {
	u8 script_type;
	const u8 *script_code;
	const u8 *annex;
	const struct sha256 *tapleaf;
	u32 codeseparator;
};

bool bitcoin_unified_sighash(const struct wally_tx *tx, size_t input,
			    u8 hash_type,
			    const struct bitcoin_tx_output *spent,
			    size_t num_spent,
			    const struct unified_sighash_input *exec,
			    struct sha256 *digest);

/* Extract spent outputs from the PSBT. Missing or inconsistent metadata fails. */
bool bitcoin_tx_unified_sighash(const struct bitcoin_tx *tx, size_t input,
			       u8 hash_type,
			       const struct unified_sighash_input *exec,
			       struct sha256 *digest);
#endif /* LIGHTNING_BITCOIN_UNIFIED_SIGHASH_H */
