# Blake2b Lightning identity proposal (experimental)

## Wire contract

Proposed name: `option_blake2b_network`.
Provisional feature pair: **4242/4243**, context **I** (init) only.
This is NOT a BOLT assignment or an agreed community standard. The published
BOLT 9 table does not assign this pair; that does not establish absence of
private experiments. Coordinate allocation with implementers before release.

The feature means this connection operates on Bitcoin Blake2b header-v2
consensus, not merely that the binary can parse Blake2b blocks. A SHA256 session
MUST NOT advertise it, including in a binary supporting both networks.

A strict Blake2b node MUST send even bit 4242 in init.features and MUST reject
peers offering neither bit of the pair before admitting them or accepting gossip.
A sender MUST NOT set both bits. Receivers accept either bit as a declaration,
consistent with BOLT feature negotiation. Peers must also provide init.networks
containing the configured genesis hash. The bit separates consensus families;
the networks field separates environments. Legacy globalfeatures do not qualify.

This implementation applies the rule to all non-Elements networks in this
Blake2b distribution. It is not a dual-consensus binary. Elements remains
unchanged. The feature is not inserted into invoices, channel types, channel
announcements or node announcements. No channel/wallet formats change.

## Why required rather than optional

Conforming SHA256 nodes reject our unknown even bit. We reject their missing
bit. An optional advertisement alone would be silently ignored by legacy nodes
and would not provide bilateral separation. All checks run on inbound and
outbound init exchange. This replaces the earlier draft custom identity TLV;
that TLV is no longer sent or recognized by this proposal.

## Trust and migration

The encrypted transport binds the declaration to the authenticated node key.
It does not prove software behavior, backend validation or honesty. A malicious
peer can copy the feature bit. Existing funding validation remains necessary;
imported gossip is not isolated by this admission rule.

Unpatched Blake2b nodes are rejected too, including existing channel partners.
Coordinate upgrades with every partner before deploying. No existing-channel
bypass is provided. Disconnection does not migrate or close a channel, but
prolonged loss of connectivity can require on-chain recovery and fees.

The source branch does not change Paperclip's running node. Before release,
agree on the bit allocation, publish cross-implementation wire vectors, and test
inbound/outbound admission, reconnects, missing/odd/even features, mismatched
networks, and channel recovery in isolated environments. Never label the draft
production-ready on the strength of a unit test alone.

## Validation

Focused CI builds connectd and lightningd and exercises normal feature rejection,
feature placement and generated init wire serialization. Funded-channel recovery
and cross-implementation integration remain release gates.
