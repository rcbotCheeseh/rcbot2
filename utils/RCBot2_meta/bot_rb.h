#ifndef __RCBOT_RB_H__
#define __RCBOT_RB_H__

class CBotRule
{
public:
	CBotRule ()
	{
	}
private:
	vector<CBotOperator> m_Rules;
};

class CBotOperator
{
public:
	CBotOperator ( CBotFactOpertor op ) : m_op(op)
	{		
	}

	virtual bool operate ( bool bVal, CBotOperator *pNext )
	{
		switch (m_op)
		{
		case OP_NONE:
			return bVal;
		case OP_PRE_NORM:
			return pNext->value();
		case OP_PRE_NOT:
		case OP_AND:
		case OP_OR:
		case OP_AND_NOT:
		case OP_OR_NOT:
		}
	}

	virtual bool value()
	{
		return -1;
	}
private:
	CBotFactOperator m_op;
};
// for use with rule list
class CBotFact : public CBotOperator
{
public:
	CBotFact ( unsigned int iFactId ) : m_fid(iFactId)
	{
	}

	bool operate ( bVal, CBotOperator *pNext )
	{
		return m_bVal;
	}

	bool value()
	{
		return m_bVal;
	}
private:
	unsigned int m_fid;
	bool m_bVal;
};

typedef enum 
{
	OP_NONE = 0,
	OP_PRE_NORM,
	OP_PRE_NOT,
	OP_AND,
	OP_OR,
	OP_AND_NOT,
	OP_OR_NOT
}CBotFactOperator;

#endif