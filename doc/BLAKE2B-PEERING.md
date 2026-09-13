# Blake2b peer identity (experimental v1)

This branch requires a consensus-family declaration during encrypted BOLT 1
`init`, before admitting a Bitcoin peer. Both inbound and outbound connections
must include custom odd TLV type 1280934931 with the exact ASCII bytes
`bitcoin-blake2b/header-v2/1` (no NUL), plus a `networks` entry matching the
configured genesis hash. Missing, empty, malformed or different identities fail
closed. Elements behavior is unchanged.

The odd custom type allows an ordinary implementation to parse the message;
our receiver still rejects its missing declaration. This is an experimental
identifier, not an assigned BOLT standard. Other Blake2b implementations must
agree on its type and value before interoperating. It distinguishes the
consensus family, while the existing networks field distinguishes environments.

## Limits

The encrypted transport binds the declaration to the authenticated node key.
It is NOT proof of chain validation, an allowlist, or protection against a
malicious peer copying the declaration. Existing funding validation remains
necessary. The patch does not alter genesis hashes, invoice prefixes, channel
IDs, signatures, wallet data, or gossip chain hashes. Imported gossip and
compromised compatible peers are outside this admission check.

## Existing channels and deployment

Unpatched Blake2b peers are rejected too. Coordinate upgrades with EVERY channel
partner before deploying. An existing channel does not bypass the check.
Disconnection is not a channel migration or automatic cooperative close;
prolonged loss of connectivity may require on-chain recovery and incur fees.
Do not deploy to funded nodes until isolated connection, reconnection and
channel recovery tests pass. No production Paperclip node is changed by this
source branch. Preserve backups and arrange a coordinated rollback if necessary.

The focused CI builds connectd and checks generated wire round trips, missing
identity, version/length mismatch and truncation. Live two-node admission,
cross-network rejection and funded-channel recovery still require integration
testing before release.
