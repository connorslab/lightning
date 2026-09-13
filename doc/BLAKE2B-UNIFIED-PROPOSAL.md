# Paperclip unified-channel proposal, draft 0

Status: design proposal with a compiled digest foundation, NOT an enabled
channel protocol. This document is Paperclip's proposal for review, not text
approved by cguida, the BOLT authors, or Bitcoin Knots. No production channels
have been changed. The executable MUST NOT advertise unified-channel support
until all signing, verification, persistence and recovery gates below pass.

## Basis

- privkeyio `v26.06.7-blake2b.3` remains the base, including schema 284.
- cguida's rescue plan requires an odd Blake2b declaration in peer/node
  contexts, even payment-document markers, shared genesis during recovery,
  and unified signatures on every new Blake2b channel.
- Consensus reference: Bitcoin Knots PR 357, source commit
  `54d757f269d21e784c771497e0a26b35ab7d0c5a`,
  `doc/unified-sighash.md` and `src/test/data/unified_sighash.json`.
  Pin and compare with the deployment's final Knots tag before activation.

## Two different capabilities

`option_blake2b` identifies the ecosystem; it does not prove signing capability.
Keep the existing temporary 4110/4111 pair for this proposal, odd in init/node
announcements and even in BOLT11/12 documents. The requested final >=32768
allocation conflicts with BOLT11's 5115-bit field maximum. This remains an
explicit registry/spec issue, not something to hide with a new invoice format.

Propose `option_unified_sigs` at TEMPORARY 32768/32769 for review. This pair is
NOT allocated or advertised by this build. Unlike the ecosystem marker it is
not copied into BOLT11/12, so the invoice-field limit does not apply to it.
It is optional (32769) in init; the even bit (32768) is a persistent channel-type
modifier. Do not use an even init bit, which would break recovery connectivity.
Both peers must offer it and `option_blake2b`; support for explicit
`option_channel_type` negotiation is required. Never infer it from a node
announcement, alias, DNS name, backend URL, or ecosystem marker alone.

Proposed valid channel types: static_remotekey, or static_remotekey plus
anchors_zero_fee_htlc, each with unified_sigs; existing scid_alias and zeroconf
variants retain their meanings. Never retrofit the modifier while decoding an
old database row. Absence means legacy, including after software upgrade.

## Opening and policy

All NEW Blake2b channels MUST negotiate unified_sigs in both open/accept
channel_type fields before exchanging funding signatures. A missing modifier,
missing capability, unsupported variant or downgrade MUST fail without funding.
This applies to inbound and outbound opens, single- and dual-funded channels,
manual channel_type overrides, plugins and reconnect/resume paths.

The existing migration/strict setting remains a PEER policy only. Migration
may connect to a legacy peer for recovery but must not authorize a new legacy
channel. Strict may reject that connection altogether. Neither setting may
disable unified signing for a channel already negotiated as unified.

New-channel capability is only enabled after the implementation gates below.
Do not change the advertised feature before the signer can enforce it.

## Signature contract

ECDSA/Schnorr key algorithms and Lightning payment hashes are unchanged.
The signature digest is Knots' TaggedHash("UnifiedSighash", message), not
BIP143 with an extra flag. The hash-type byte includes 0x20:

| Path | Required hash type |
| --- | --- |
| Commitments and ordinary all-output signatures | ALL\|UNIFIED = 0x21 |
| Anchor HTLC second-stage signatures | SINGLE\|ANYONECANPAY\|UNIFIED = 0xa3 |
| Funding/splice wallet inputs and cooperative closes | Unified counterpart of the permitted path's base type |
| Unilateral resolution, delayed outputs, HTLC claims and justice | Unified counterpart of the permitted path's base type |

External co-signers must supply unified funding signatures where controlled by
this protocol. Funding witnesses and remote signatures are checked before
broadcast or state acceptance. No silent fallback to 0x01/0x83 is allowed for a
unified channel. Lightning's compact wire signatures do not carry a sighash
byte: both sides reconstruct it from the persisted channel type and transaction
role. DER/witness serialization must append the reconstructed byte.

The HSM must independently obtain persisted channel signing policy, authenticate
the funding outpoint and all required prevout amounts/scripts, and reject legacy
requests for unified channels. Do not trust a caller-supplied boolean to change
the policy. ANYONECANPAY commits only the selected input's prevout metadata;
other signatures need metadata for every input. Missing metadata is an error,
never a zero amount, empty script or legacy fallback.

Do not change invoice signatures, node/gossip signatures, or payment preimages.
Wallet signing and PSBT handling require separate end-to-end plumbing, including
taproot. Legacy recovery signatures remain explicitly legacy when resolving an
old channel; they must never be reported as replay-protected.

## Existing channels: proposed upgrade protocol

A local setting cannot rewrite the remote party's signatures, erase old signed
transactions, or retroactively make a legacy channel replay-safe. This proposal
therefore distinguishes LEGACY, UPGRADING, UNIFIED and RECOVERY_ONLY states.
It does not automatically close channels or spend funds.

Provisional extension for review: an odd `channel_reestablish` TLV 32769 named
`unified_upgrade`, containing version:u16=0, phase:u8, upgrade_id:32 bytes,
target_type_hash:32 bytes, local_commitment_number:u64,
remote_commitment_number:u64 and state_hash:32 bytes. Integer encoding follows
BOLT wire conventions. These are test allocations, not registry assignments.
Unknown versions, inconsistent state or unexpected phases stop the upgrade;
they do not authorize a state transition. No secret is carried in this TLV.

Detailed signature-transfer TLVs and the exact canonical snapshot encoding
must be frozen with interoperability vectors before implementing this wire
proposal. Reestablish is a resumable checkpoint advertisement, not by itself
permission to revoke or sign. The proposed state machine is:

1. Both peers explicitly consent; complete normal reestablish first. Enter
   negotiated quiescence. Version 0 requires zero outstanding HTLCs, no pending
   updates/revocations, no funding/splice/RBF attempt, and no shutdown in flight.
2. Agree an upgrade ID bound to both node IDs in canonical order, funding
   outpoint, existing/target channel types, both commitment counters, balances,
   fee state and a fresh nonce from each peer. Canonical encoding must be
   specified; JSON or implementation-native memory layouts are forbidden.
3. Construct the next commitment state in each direction using fresh standard
   per-commitment points and unified signatures. Exchange and verify replacement
   signatures before revealing any old revocation secret. Persist the full
   replacement, transcript, counters and phase atomically before acknowledging.
4. Only after the local replacement is enforceable and durably stored may the
   node revoke its old local state. Retain both chains' recovery/justice data.
   Never interpret transport delivery or an acknowledgment as signature proof.
5. Resume normal HTLC operation only after both sides have completed the agreed
   revocation exchange and durably recorded UNIFIED. Future signatures must use
   the persisted new type, including after restart, backup recovery and splice.

Crash before revocation: keep the old enforceable state and stay quiescent while
resuming or safely abandoning the attempt. Crash after any revocation: never
roll back to the old state; resume from the durable transcript or use the valid
replacement. Duplicate/reordered frames must be idempotent or rejected.
No rollback may change a UNIFIED channel to LEGACY.

There is a material cross-chain limitation: revocation does NOT invalidate an
old commitment on either chain. Enforcement depends on timely penalty spends.
Unified penalties alone cannot punish replay on the SHA256 side. Accordingly,
the in-place upgrade is not enabled without the dual-chain watcher, retained
legacy justice material and settlement/recovery design required by the rescue
plan. Otherwise the conservative migration path is coordinated closure and
opening a new unified channel, with explicit funds authorization. Even that
does not retroactively settle the other chain's funding output.

## Implementation status and release gates

Implemented in this branch:
- Blake2b signaling, marked payment readers/writers and configurable peering.
- Standalone compiled `bitcoin_unified_sighash` foundation with explicit spent
  outputs and script context; no channel/HSM caller is switched to it yet.
- The 166 Knots digest vectors for script types 0/1 and basic rejection tests.
  Taproot/annex/code-separator extensions need independent vectors; digest
  tests are not channel enforceability tests.

Not implemented or advertised:
- unified_sigs channel negotiation/enforcement, HSM/wallet integration,
  durable upgrade state machine and dual-chain recovery. Existing executable
  channel behavior remains upstream legacy signing. This binary therefore does
  NOT yet satisfy the requirement that every channel use SIGHASH_UNIFIED.

Required before enabling capability: independently checked digest/signature
vectors; funding/commitment/HTLC/close/justice/splice tests accepted by active
Blake2b Knots and rejected by a chain without unified sighash; wrong-prevout and
downgrade tests; crash/restart at every transition; backup restoration;
independent implementation interoperability; and replay/penalty tests on both
chains. Unsupported paths must be refused, not silently signed using old rules.
