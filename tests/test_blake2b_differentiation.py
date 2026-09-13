"""Mandatory peer feature without invoice modifications."""
from fixtures import *  # noqa: F401,F403
from pyln.client import RpcError
import pytest


@pytest.mark.parametrize('incoming', [False, True])
def test_blake2b_required_peer_bit(node_factory, incoming):
    blake, legacy = node_factory.get_nodes(2, opts=[
        {'may_reconnect': True, 'allow_warning': True},
        {'dev-force-features': '-68', 'may_reconnect': True, 'allow_warning': True}])
    source, target = (legacy, blake) if incoming else (blake, legacy)
    with pytest.raises(RpcError):
        source.rpc.connect(target.info['id'], 'localhost', target.port)
    assert not any(p['connected'] for p in blake.rpc.listpeers()['peers'])
    for source, reader in [(blake, legacy), (legacy, blake)]:
        invoice = source.rpc.invoice(1000, 'compat', 'invoice compatibility')['bolt11']
        assert reader.rpc.decode(invoice)['valid']


def test_blake2b_peers_connect(node_factory):
    a, b = node_factory.get_nodes(2)
    a.rpc.connect(b.info['id'], 'localhost', b.port)
    assert a.rpc.listpeers()['peers'][0]['connected']
