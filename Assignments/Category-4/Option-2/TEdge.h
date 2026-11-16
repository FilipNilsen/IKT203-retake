#pragma once
#include <cfloat>

class TNode;
class TEdge
{
	public: float weigth = FLT_MAX;
	public: TNode* end = nullptr;
	public: TEdge(TNode* _end, float _weigth)
	{
		this->weigth = _weigth;
		this->end = _end;
	}
};