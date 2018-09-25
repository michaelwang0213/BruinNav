#ifndef MY_MAP
#define MY_MAP
template<typename KeyType, typename ValueType>
class MyMap
{
public:
	struct node
	{
		node(KeyType firstVal, ValueType secondVal)
		{
			first = firstVal;
			second = secondVal;
			left = nullptr;
			right = nullptr;
		}
		KeyType first;
		ValueType second;

		node *left;
		node *right;
	};

	MyMap()
	{
		m_root = nullptr;
	}
	~MyMap()
	{
		clear();
	}
	void clear()
	{
		clearHelper(m_root);
	}
	int size() const
	{
		return sizeHelper(m_root);
	}
	//	The	associate	method associates one item (key) with another (value).
	//	If	no	association	currently	exists	with	that	key, this	method	inserts
	//	a	new	association	into	the	tree	with	that	key/value	pair.		If	there	is
	//	already	an	association	with	that	key	in	the	tree,	then	the	item
	//	associated	with	that	key	is	replaced	by	the	second	parameter	(value).
	//	Thus,	the	tree	contains	no	duplicate	keys.
	void associate(const KeyType& key, const ValueType& value)
	{
		if (m_root == nullptr)
		{
			m_root = new node(key, value);
			return;
		}
		node *cur = m_root;
		for (;;)
		{
			if (key == cur->first)
			{
				cur->second = value;
				return;
			}
			if (key < cur->first)
			{
				if (cur->left != nullptr)
					cur = cur->left;
				else
				{
					cur->left = new node(key, value);
					return;
				}
			}
			else if (key > cur->first)
			{
				if (cur->right != nullptr)
					cur = cur->right;
				else
				{
					cur->right = new node(key, value);
					return;
				}
			}
		}
	}

	// for a map that can't be modified, return a pointer to const ValueType
	//	If	no	association	exists	with	the	given	key,	return	nullptr;	otherwise,
	//	return	a	pointer	to	the	value	associated	with	that	key.		This	pointer	can	be
	//	used	to	examine	that	value,	and	if	the	map	is	allowed	to	be	modified,	to
	//	modify	that	value	directly	within	the	map (the	second	overload	enables
	//	this).		Using	a	little	C++	magic,	we	have	implemented	it	in terms	of	the
	//	first	overload,	which	you	must	implement.
	const ValueType* find(const KeyType& key) const
	{
		return findHelper(key, m_root);
	}

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;


	/////////////////////////////////////////////////////
	// Prints out all values in the map in order
	// To be deleted after testing
	/////////////////////////////////////////////////////
	/*void display() const
	{
		displayHelper(m_root);
	}*/

private:
	node *m_root;
	// Implements a postorder traversal to only delete nodes without children
	void clearHelper(node *cur)
	{
		if (cur == nullptr)
			return;
		clearHelper(cur->left);
		clearHelper(cur->right);
		delete cur;
	}
	// Recursively counts the number of nodes in the tree by returning one plus the number of
	// nodes in the left sub tree plus the number of nodes in the right subtree.
	int sizeHelper(node *cur) const
	{
		if (cur == nullptr)
			return 0;
		return 1 + sizeHelper(cur->left) + sizeHelper(cur->right);
	}
	// Recursively searches the binary search tree for a given key value
	ValueType* findHelper(const KeyType key, node *cur) const
	{
		if (cur == nullptr)
			return nullptr;
		else if (key == cur->first)
			return &(cur->second);
		else if (key < cur->first)
			return findHelper(key, cur->left);
		else
			return findHelper(key, cur->right);
	}
	// For testing purposes only. Uses in order.
	void displayHelper(node *cur) const
	{
		if (cur == nullptr)
			return;
		displayHelper(cur->left);
		//cerr << cur->first << " : " << cur->second << endl;
		displayHelper(cur->right);
	}
};
#endif

