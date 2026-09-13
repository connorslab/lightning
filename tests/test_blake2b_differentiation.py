"""Peer declaration does not alter invoice compatibility."""
from fixtures import *  # noqa: F401,F403


def test_blake2b_declaration_preserves_invoices(node_factory):
    blake, legacy = node_factory.get_nodes(2, opts=[{}, {'dev-force-features': '-68'}])
    blake.rpc.connect(legacy.info['id'], 'localhost', legacy.port)
    assert blake.rpc.listpeers()['peers'][0]['connected']
    for source, reader in [(blake, legacy), (legacy, blake)]:
        invoice = source.rpc.invoice(1000, 'compat', 'invoice compatibility')['bolt11']
        assert reader.rpc.decode(invoice)['valid']
    blake.rpc.disconnect(legacy.info['id'])
    legacy.rpc.connect(blake.info['id'], 'localhost', blake.port)
    assert legacy.rpc.listpeers()['peers'][0]['connected']
