import ctypes
import unittest


# --- ctypes bindings ---

lib = ctypes.CDLL("./LinearTree.so")


class Node(ctypes.Structure):
    _fields_ = [
        ("val", ctypes.c_int),
        ("left", ctypes.c_uint32),
        ("right", ctypes.c_uint32),
    ]


class Tree(ctypes.Structure):
    _fields_ = [
        ("root", ctypes.POINTER(Node)),
        ("tr", ctypes.c_void_p),  # opaque __tree_internals*
    ]


lib.treeCreate.argtypes = []
lib.treeCreate.restype = ctypes.POINTER(Tree)

lib.insertValue.argtypes = [ctypes.POINTER(Tree), ctypes.c_int]
lib.insertValue.restype = ctypes.c_bool

lib.left.argtypes = [ctypes.POINTER(Tree), ctypes.POINTER(Node)]
lib.left.restype = ctypes.POINTER(Node)

lib.right.argtypes = [ctypes.POINTER(Tree), ctypes.POINTER(Node)]
lib.right.restype = ctypes.POINTER(Node)


# --- helpers ---


def collect_inorder(tree_ptr, node_ptr):
    """Walk the tree in-order and return sorted list of values."""
    if not node_ptr:
        return []
    result = []
    result += collect_inorder(tree_ptr, lib.left(tree_ptr, node_ptr))
    result.append(node_ptr.contents.val)
    result += collect_inorder(tree_ptr, lib.right(tree_ptr, node_ptr))
    return result


# --- tests ---


class TestTreeCreate(unittest.TestCase):
    def test_create_returns_nonnull(self):
        tree = lib.treeCreate()
        self.assertIsNotNone(tree)
        self.assertTrue(bool(tree))

    def test_root_initially_null(self):
        tree = lib.treeCreate()
        self.assertFalse(bool(tree.contents.root))


class TestInsertValue(unittest.TestCase):
    def setUp(self):
        self.tree = lib.treeCreate()

    def test_insert_single_value(self):
        ok = lib.insertValue(self.tree, 10)
        self.assertTrue(ok)

    def test_root_set_after_first_insert(self):
        lib.insertValue(self.tree, 10)
        self.assertTrue(bool(self.tree.contents.root))
        self.assertEqual(self.tree.contents.root.contents.val, 10)

    def test_insert_duplicate_returns_false(self):
        lib.insertValue(self.tree, 5)
        ok = lib.insertValue(self.tree, 5)
        self.assertFalse(ok)

    def test_insert_multiple_values(self):
        values = [5, 3, 7, 1, 4]
        for v in values:
            lib.insertValue(self.tree, v)
        inorder = collect_inorder(self.tree, self.tree.contents.root)
        self.assertEqual(inorder, sorted(values))

    def test_insert_returns_false_on_null_tree(self):
        null_tree = ctypes.POINTER(Tree)()
        ok = lib.insertValue(null_tree, 1)
        self.assertFalse(ok)


class TestTraversal(unittest.TestCase):
    def setUp(self):
        self.tree = lib.treeCreate()
        for v in [10, 5, 15, 3, 7]:
            lib.insertValue(self.tree, v)
        self.root = self.tree.contents.root

    def test_left_child_of_root(self):
        node = lib.left(self.tree, self.root)
        self.assertIsNotNone(node)
        self.assertEqual(node.contents.val, 5)

    def test_right_child_of_root(self):
        node = lib.right(self.tree, self.root)
        self.assertIsNotNone(node)
        self.assertEqual(node.contents.val, 15)

    def test_left_returns_null_for_leaf(self):
        leaf = lib.left(self.tree, self.root)  # node 5
        left_of_leaf = lib.left(self.tree, leaf)
        # node 3 exists, so left of 5 should be 3
        self.assertEqual(left_of_leaf.contents.val, 3)

    def test_right_returns_null_for_rightmost_leaf(self):
        right_of_15 = lib.right(self.tree, lib.right(self.tree, self.root))
        self.assertFalse(bool(right_of_15))

    def test_inorder_is_sorted(self):
        inorder = collect_inorder(self.tree, self.root)
        self.assertEqual(inorder, sorted(inorder))


if __name__ == "__main__":
    unittest.main(verbosity=2)
