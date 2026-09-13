# Paperclip Blake2b differentiation proposal

This is an experimental Ubuntu 24.04 amd64 build based on
v26.06.7-blake2b.3. It adds actual feature signaling, payment-document checks
and a configurable local peer-admission policy. It is not a full migration
implementation or a registered protocol release. Do not install over funded
nodes until partner coordination and channel-recovery tests are complete.

Configuration (restart required; no production node changed by building):

    blake2b-peer-policy=migration

Default. Legacy peer connections remain possible, following the migration plan.
This is not recovery-only enforcement: normal legacy channel behavior remains.

    blake2b-peer-policy=strict

Reject inbound/outbound init messages lacking option_blake2b or a matching
networks list, before peer admission. Existing peers have no exemption. For
fresh or fully migrated nodes only: enabling it can strand legacy channel
partners. It does not close channels automatically.

Both modes advertise an odd init/node bit, and an even BOLT11/12 payment bit.
See PROPOSAL.md for semantics and limitations. The temporary pair 4110/4111
comes from the plan's placeholders; it is NOT the final registry allocation.

The shared genesis and payment hashes remain unchanged. No unified signing,
channel type upgrade, migration daemon, replay protection or custom handshake
message is added. Keysend/raw RPC bypass of invoice semantics is not a proof of
network identity; do not use these to infer chain compatibility. Malicious peers
can copy the feature bit. Strict mode separates honest differently configured
peers, not adversaries claiming false capabilities.

SHA256SUMS verifies archive integrity; it is not a publisher signature.
SOURCE-COMMIT identifies the exact source. Runtime OS dependencies must be
installed separately. This development build uses the Rust small profile.

Upstream .3 corrects the .2 base from CLN 26.06.6 to 26.06.7, including its
fixes and database schema 284 (previously 282). An existing 282 database needs
an irreversible upgrade, which this development version may require explicitly
authorizing with --database-upgrade=true. Back up the full node state first;
afterward .2 cannot open that database. This build does not deploy or migrate it.

UNIFIED-PROPOSAL.md describes draft channel negotiation and migration.
The compiled unified digest foundation is not connected to HSM/channel signing.
This executable DOES NOT yet enforce SIGHASH_UNIFIED on channels and MUST NOT
be represented as implementing the unified-channel requirement. It does not
advertise option_unified_sigs. See the proposal's explicit implementation gates.
