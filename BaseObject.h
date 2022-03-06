#pragma once
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "CommonFunction.h"

class BaseObject {
public:
	BaseObject();
	~BaseObject();
	// Gán
	void SetRect(const int& x, const int& y) {
		rect_.x = x;
		rect_.y = y;
	}
	// Lấy rect
		SDL_Rect GetRect() const { return rect_; }
		SDL_Texture* GetObject() const { return p_object_;}

		virtual bool LoadImg(std::string path, SDL_Renderer* screen);
		void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
		void Free();
protected:
	SDL_Texture* p_object_;
	SDL_Rect rect_;

};
#endif