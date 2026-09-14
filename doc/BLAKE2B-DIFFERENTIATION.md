# Paperclip changes

Base: privkeyio v26.06.7-blake2b.3.

1. A Blake2b declaration: provisional feature pair 68/69, with required bit 68
   in init and node announcements. Not a registered BOLT assignment.
2. Unified sighash signing for wallet transactions and new channels. The digest
   passes 166 Knots vectors, and isolated Knots tests cover funding, payments,
   restart, cooperative/unilateral closes, withdrawal, and splicing using both
   channel-opening protocols. Existing channels are not automatically migrated.

Invoice creation, parsing and payment behavior are unchanged from privkeyio.
There is no custom peer-admission mode or changed genesis identifier.
Cguida's broader plan includes invoice markers and channel migration; those are
outside this narrowed patch. The peer bit is a declaration, not replay protection.

New channels use provisional channel-type bit 70 (option_unified_sigs), announced
as capability bit 71 in init. Their transaction signatures use the actual Knots
UnifiedSighash digest and flag 0x20. Both feature assignments need coordination.

Required bit 68 is an explicit operator-requested departure from cguida's
optional migration signaling. Older peers cannot connect without upgrading.
