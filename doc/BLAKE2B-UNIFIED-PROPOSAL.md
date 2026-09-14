# Unified signing scope

Use Bitcoin Knots SIGHASH_UNIFIED (0x20) and its UnifiedSighash tagged digest.
The existing digest implementation passes all 166 Knots vectors across the four
script types. The matching final Knots reference is v29.4.1.knots20260508.

The digest is connected to wallet and channel signing and verification.
New unified channels persist channel-type bit 70 across restarts. Wallet signing
uses unified ECDSA or Schnorr signatures as appropriate; existing legacy channel
types retain their original signing rules.

CI run 34790932048 passed on source commit
167e2196f (full hash is recorded in the build's SOURCE-COMMIT): five peer/invoice
compatibility tests and six Knots transaction tests, including both channel-opening
protocols and splicing. The digest suite also verifies that a unified ECDSA
signature fails verification against the legacy BIP143 digest.

This is an unofficial test implementation, not an exhaustive validation of every
HTLC recovery, penalty, or migration scenario. Use disposable test funds.

No custom channel-upgrade handshake is included in this narrowed patch.
Existing signed transactions cannot be retroactively protected by adding a bit.
No live node or funded channel has been modified.
