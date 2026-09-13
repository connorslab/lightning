# Differentiation proposal and interoperability contract

## Basis and scope

Implements the signaling direction described in cguida's rescue/grand-plan
material: optional peer/node signaling, required payment-document signaling,
shared chain_hash through migration. It does not implement the broader rescue
or service fabric. Strict local admission is a USER-CONFIGURABLE extension,
not the migration plan's default or a new mandatory wire feature.

## Allocation issue

Name: option_blake2b. TEST ONLY pair: 4110/4111 (the plan's legacy placeholder).
Do not claim it is reserved. The plan requests final feature numbers >=32768,
but BOLT11 tagged fields have a 10-bit length measured in 5-bit groups: maximum
1023 * 5 = 5115 bits. A feature at index 32768 cannot fit. Resolve this mismatch
in the spec venue before assigning the final interoperable feature(s). This
proposal does not silently introduce a custom invoice format to evade it.

## Wire rules

- init.features and node_announcement.features: bit 4111 (optional).
- BOLT11 features, BOLT12 offer/invoice-request/invoice features: bit 4110
  (required). Including invoice-request is explicit in this proposal and needs
  review with the detailed BOLT12 specification.
- Never add this bit to channel_type, channel announcements or legacy
  globalfeatures. Never change genesis hash, channel IDs or signatures.
- Ordinary implementations may ignore the optional peer bit; their normal
  unknown-even-feature checks reject marked payment documents.
- Our payment readers require the even marker. Missing and optional-only
  payment markers are rejected, including invoices omitting features entirely.
- Inspection-only decoders that deliberately supply no supported-feature set
  can still inspect arbitrary documents. That does not authorize payment.

## Local peer policy

Migration is the default and preserves legacy connectivity, including networks
TLV omission permitted by the baseline. It does not implement a recovery-only
message firewall or disable legacy HTLC updates: migration safety needs the
separate coordinator/unified-signature work.

Strict rejects missing feature or missing/mismatched networks after decrypting
init and before registering the peer. This applies in both directions, also
on reconnection. Both optional and required forms of the pair count as a peer
declaration. It changes neither the outbound odd bit nor global gossip IDs.
Elements does not advertise the bit and rejects strict-policy configuration.

Node key authentication binds a declaration to its sender, not to honest chain
validation. Same-key peers can lie; unified-signature and funding verification
remain separate. Preexisting gossip databases are not separated by this patch.
Do not use a previously SHA256-funded node as a fresh strict node without the
migration/recovery work. No claim of replay-safe channels is made.

## Tests and release gates

Unit tests cover parity/context placement, missing markers, legacy unknown-bit
handling and wire round trips. Isolated regtest tests cover legacy-compatible
connections, strict inbound/outbound rejection, compatible reconnection and
invoice parsing. These are protocol checks on an isolated Bitcoin regtest,
not Blake2b mainnet consensus validation or a funded-channel migration test.

Before production: agreed registry/spec resolution, independent LND/Eclair
interop, signing/recovery vectors, peer upgrade coordination, backup/restore
and force-close tests, and review of existing invoices/offers made before upgrade
(they lack the marker and will be refused by marked-payment readers).
