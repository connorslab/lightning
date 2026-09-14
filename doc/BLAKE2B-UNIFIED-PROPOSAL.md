# Unified signing scope

Use Bitcoin Knots SIGHASH_UNIFIED (0x20) and its UnifiedSighash tagged digest.
The existing digest implementation passes all 166 Knots vectors across the four
script types. The matching final Knots reference is v29.4.1.knots20260508.

Transaction signing and verification now include wallet and channel paths.
The dedicated build tests cover funding, payments, restart, cooperative and
unilateral close, withdrawal and splicing against the matching Knots backend.
These checks supplement the digest vectors; the full inherited CI suite remains
under validation.

No custom channel-upgrade handshake is included in this narrowed patch.
Existing signed transactions cannot be retroactively protected by adding a bit.
An isolated, unfunded test node runs alongside Paperclip's existing node.
The existing node and funded channels have not been migrated to this build.
