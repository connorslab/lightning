# Unified signing scope

Use Bitcoin Knots SIGHASH_UNIFIED (0x20) and its UnifiedSighash tagged digest.
The existing digest implementation passes all 166 Knots vectors across the four
script types. The matching final Knots reference is v29.4.1.knots20260508.

The active task is connecting this to transaction signing and verification,
including the wallet and channel paths. A digest test alone does not establish
that an actual wallet or channel transaction is replay-protected.

No custom channel-upgrade handshake is included in this narrowed patch.
Existing signed transactions cannot be retroactively protected by adding a bit.
No live node or funded channel has been modified.
