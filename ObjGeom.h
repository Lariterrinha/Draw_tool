/* Copyright (c) 2024 Lilian Buzer - All rights reserved - */
#pragma once

#include "Color.h"
#include "V2.h"
#include "ObjAttr.h"
#include "Graphics.h"
#include <sstream>
#include <memory>
#include <vector>


class ObjGeom
{
public :
	ObjAttr drawInfo_;

	ObjGeom() {}
	ObjGeom(ObjAttr  drawInfo) : drawInfo_(drawInfo)   {  }

	virtual void draw(Graphics & G) {}

	virtual void getBoundingBox(V2& P, V2& size) const { P = V2(0,0); size = V2(0,0); }

	virtual bool contains(const V2& p) const
	{
		V2 P; V2 size;
		getBoundingBox(P, size);
		return (p.x >= P.x && p.x <= P.x + size.x && p.y >= P.y && p.y <= P.y + size.y);
	}

	virtual void getControlPoints(std::vector<V2>& out) const { }

	virtual V2* findClosestControlPoint(const V2& mouse, float maxDist) { return nullptr; }
	virtual std::string serialize() const = 0;   
	static std::shared_ptr<ObjGeom> deserialize(const std::string& line);

};


class ObjRectangle : public ObjGeom
{
public :
	V2 P1_;
	V2 P2_;

	ObjRectangle(ObjAttr  drawInfo, V2 P1, V2 P2) : ObjGeom(drawInfo)
	{
		P1_ = P1;
		P2_ = P2;
	}

	void draw(Graphics& G) override
	{
		V2 P, size;
		getPLH(P1_, P2_, P, size);

		if ( drawInfo_.isFilled_ )
  		   G.drawRectangle(P,size, drawInfo_.interiorColor_, true);

		G.drawRectangle(P, size, drawInfo_.borderColor_, false, drawInfo_.thickness_);
	}

	void getBoundingBox(V2& P, V2& size) const override
	{
		getPLH(P1_, P2_, P, size);
	}

	bool contains(const V2& p) const override
	{
		V2 P; V2 size;
		getBoundingBox(P, size);
		return (p.x >= P.x && p.x <= P.x + size.x && p.y >= P.y && p.y <= P.y + size.y);
	}
	std::string serialize() const override
	{
		std::ostringstream ss;
		ss << "RECT "
			<< drawInfo_.borderColor_.R << ' ' << drawInfo_.borderColor_.G << ' '
			<< drawInfo_.borderColor_.B << ' ' << drawInfo_.borderColor_.A << ' '
			<< drawInfo_.isFilled_ << ' '
			<< drawInfo_.interiorColor_.R << ' ' << drawInfo_.interiorColor_.G << ' '
			<< drawInfo_.interiorColor_.B << ' ' << drawInfo_.interiorColor_.A << ' '
			<< drawInfo_.thickness_ << ' '
			<< P1_.x << ' ' << P1_.y << ' ' << P2_.x << ' ' << P2_.y;
		return ss.str();
	}

	void getControlPoints(std::vector<V2>& out) const override
	{
		out.push_back(P1_);
		out.push_back(P2_);
	}

	V2* findClosestControlPoint(const V2& mouse, float maxDist) override
	{
		V2* best = nullptr;
		double bestDist = maxDist;

		double d = (P1_ - mouse).norm();
		if (d <= bestDist) { bestDist = d; best = &P1_; }

		d = (P2_ - mouse).norm();
		if (d <= bestDist) { bestDist = d; best = &P2_; }

		return best;
	}

};


class ObjSegment : public ObjGeom
{
public:
	V2 P1_;
	V2 P2_;

	ObjSegment(ObjAttr  drawInfo, V2 P1, V2 P2) : ObjGeom(drawInfo)
	{
		P1_ = P1;
		P2_ = P2;
	}

	void draw(Graphics& G) override
	{
		V2 P, size;
	 
		G.drawLine(P1_, P2_, drawInfo_.borderColor_, drawInfo_.thickness_);
	}

	void getBoundingBox(V2& P, V2& size) const override
	{
		int xmin = std::min(P1_.x, P2_.x);
		int ymin = std::min(P1_.y, P2_.y);
		int xmax = std::max(P1_.x, P2_.x);
		int ymax = std::max(P1_.y, P2_.y);
		int pad = std::max(4, drawInfo_.thickness_);
		xmin -= pad; ymin -= pad; xmax += pad; ymax += pad;
		P = V2(xmin, ymin);
		size = V2(xmax - xmin, ymax - ymin);
	}

	bool contains(const V2& p) const override
	{
		// distância ponto->segmento com tolerância
		double x0 = p.x, y0 = p.y;
		double x1 = P1_.x, y1 = P1_.y;
		double x2 = P2_.x, y2 = P2_.y;
		double dx = x2 - x1, dy = y2 - y1;
		if (dx == 0 && dy == 0) {
			double d2 = (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1);
			double tol = drawInfo_.thickness_ + 3;
			return d2 <= tol*tol;
		}
		double t = ((x0-x1)*dx + (y0-y1)*dy) / (dx*dx + dy*dy);
		if (t < 0) t = 0; else if (t > 1) t = 1;
		double px = x1 + t*dx, py = y1 + t*dy;
		double d2 = (x0-px)*(x0-px) + (y0-py)*(y0-py);
		double tol = drawInfo_.thickness_ + 4;
		return d2 <= tol*tol;
	}
	std::string serialize() const override
	{
		std::ostringstream ss;
		ss << "SEG "
			<< drawInfo_.borderColor_.R << ' ' << drawInfo_.borderColor_.G << ' '
			<< drawInfo_.borderColor_.B << ' ' << drawInfo_.borderColor_.A << ' '
			<< drawInfo_.isFilled_ << ' '
			<< drawInfo_.interiorColor_.R << ' ' << drawInfo_.interiorColor_.G << ' '
			<< drawInfo_.interiorColor_.B << ' ' << drawInfo_.interiorColor_.A << ' '
			<< drawInfo_.thickness_ << ' '
			<< P1_.x << ' ' << P1_.y << ' ' << P2_.x << ' ' << P2_.y;
		return ss.str();
	}
	void getControlPoints(std::vector<V2>& out) const override
	{
		out.push_back(P1_);
		out.push_back(P2_);
	}

	V2* findClosestControlPoint(const V2& mouse, float maxDist) override
	{
		V2* best = nullptr;
		double bestDist = maxDist;

		double d = (P1_ - mouse).norm();
		if (d <= bestDist) { bestDist = d; best = &P1_; }

		d = (P2_ - mouse).norm();
		if (d <= bestDist) { bestDist = d; best = &P2_; }

		return best;
	}

};


class ObjCircle : public ObjGeom
{
public:
	V2 P1_;
	V2 P2_;

	ObjCircle(ObjAttr  drawInfo, V2 P1, V2 P2) : ObjGeom(drawInfo)
	{
		P1_ = P1;
		P2_ = P2;
	}

	void draw(Graphics& G) override
	{
		V2 size;

		int r;

		V2 diff = P2_ - P1_;
		r = (int)diff.norm();

		// circulo 
		if (drawInfo_.isFilled_)
			G.drawCircle(P1_, r, drawInfo_.interiorColor_, true);

		G.drawCircle(P1_, r, drawInfo_.borderColor_, false, drawInfo_.thickness_);

	
	}

	void getBoundingBox(V2& P, V2& size) const override
	{
		V2 diff = P2_ - P1_;
		int r = (int)diff.norm();
		P = V2(P1_.x - r, P1_.y - r);
		size = V2(2*r, 2*r);
	}

	bool contains(const V2& p) const override
	{
		V2 d = p - P1_;
		double dist = d.norm();
		V2 diff = P2_ - P1_;
		double r = diff.norm();
		return dist <= r + 1.0; // toleranc
	}
	std::string serialize() const override
	{
		std::ostringstream ss;
		ss << "CIRC "
			<< drawInfo_.borderColor_.R << ' ' << drawInfo_.borderColor_.G << ' '
			<< drawInfo_.borderColor_.B << ' ' << drawInfo_.borderColor_.A << ' '
			<< drawInfo_.isFilled_ << ' '
			<< drawInfo_.interiorColor_.R << ' ' << drawInfo_.interiorColor_.G << ' '
			<< drawInfo_.interiorColor_.B << ' ' << drawInfo_.interiorColor_.A << ' '
			<< drawInfo_.thickness_ << ' '
			<< P1_.x << ' ' << P1_.y << ' ' << P2_.x << ' ' << P2_.y;
		return ss.str();
	}
	void getControlPoints(std::vector<V2>& out) const override
	{
		out.push_back(P1_);
		out.push_back(P2_);
	}

	V2* findClosestControlPoint(const V2& mouse, float maxDist) override
	{
		V2* best = nullptr;
		double bestDist = maxDist;

		double d = (P1_ - mouse).norm();
		if (d <= bestDist) { bestDist = d; best = &P1_; }

		d = (P2_ - mouse).norm();
		if (d <= bestDist) { bestDist = d; best = &P2_; }

		return best;
	}

};


class ObjPolyLine : public ObjGeom
{
	std::vector<V2> pts_;

public:
	ObjPolyLine(const ObjAttr& A, const std::vector<V2>& P)
		: ObjGeom(A), pts_(P) {}

	void draw(Graphics& G) override
	{
		for (size_t i = 0; i < pts_.size() - 1; ++i)
			G.drawLine(pts_[i], pts_[i + 1], drawInfo_.borderColor_, drawInfo_.thickness_);
	}

	bool contains(const V2& P) const override
	{
		// seleção simplificada: distância do ponto a cada segmento
		for (size_t i = 0; i < pts_.size() - 1; ++i)
		{
			V2 A = pts_[i];
			V2 B = pts_[i + 1];

			double L = (B - A).norm();
			double d = fabs((P - A).norm() + (P - B).norm() - L);
			if (d < 4.0) return true;
		}
		return false;
	}

	void getBoundingBox(V2& P, V2& size) const override
	{
		int minx = 99999, miny = 99999, maxx = -99999, maxy = -99999;
		for (auto& p : pts_)
		{
			minx = std::min(minx, p.x);
			miny = std::min(miny, p.y);
			maxx = std::max(maxx, p.x);
			maxy = std::max(maxy, p.y);
		}
		P = V2(minx, miny);
		size = V2(maxx - minx, maxy - miny);
	}
	std::string serialize() const override
	{
		std::ostringstream ss;
		ss << "POLY "
			<< drawInfo_.borderColor_.R << ' ' << drawInfo_.borderColor_.G << ' '
			<< drawInfo_.borderColor_.B << ' ' << drawInfo_.borderColor_.A << ' '
			<< drawInfo_.isFilled_ << ' '
			<< drawInfo_.interiorColor_.R << ' ' << drawInfo_.interiorColor_.G << ' '
			<< drawInfo_.interiorColor_.B << ' ' << drawInfo_.interiorColor_.A << ' '
			<< drawInfo_.thickness_ << ' ';

		ss << pts_.size() << ' ';
		for (const auto& p : pts_)
			ss << p.x << ' ' << p.y << ' ';
		return ss.str();
	}
	void getControlPoints(std::vector<V2>& out) const override
	{
		for (const auto& p : pts_)
			out.push_back(p);
	}

	V2* findClosestControlPoint(const V2& mouse, float maxDist) override
	{
		V2* best = nullptr;
		double bestDist = maxDist;

		for (auto& p : pts_)
		{
			double d = (p - mouse).norm();
			if (d <= bestDist)
			{
				bestDist = d;
				best = &p;
			}
		}
		return best;
	}

};


inline std::shared_ptr<ObjGeom> ObjGeom::deserialize(const std::string& line)
{
	std::istringstream ss(line);
	std::string type;
	if (!(ss >> type)) return nullptr;

	auto readAttr = [&](ObjAttr& attr)
		{
			Color bc, ic;
			int isFilledInt;
			int thick;

			ss >> bc.R >> bc.G >> bc.B >> bc.A;
			ss >> isFilledInt;
			ss >> ic.R >> ic.G >> ic.B >> ic.A;
			ss >> thick;

			attr = ObjAttr(bc, isFilledInt != 0, ic, thick);
		};

	if (type == "RECT")
	{
		ObjAttr a;
		readAttr(a);
		int x1, y1, x2, y2;
		ss >> x1 >> y1 >> x2 >> y2;
		return std::make_shared<ObjRectangle>(a, V2(x1, y1), V2(x2, y2));
	}
	else if (type == "SEG")
	{
		ObjAttr a;
		readAttr(a);
		int x1, y1, x2, y2;
		ss >> x1 >> y1 >> x2 >> y2;
		return std::make_shared<ObjSegment>(a, V2(x1, y1), V2(x2, y2));
	}
	else if (type == "CIRC")
	{
		ObjAttr a;
		readAttr(a);
		int x1, y1, x2, y2;
		ss >> x1 >> y1 >> x2 >> y2;
		return std::make_shared<ObjCircle>(a, V2(x1, y1), V2(x2, y2));
	}
	else if (type == "POLY")
	{
		ObjAttr a;
		readAttr(a);
		size_t n;
		ss >> n;
		std::vector<V2> pts;
		pts.reserve(n);
		for (size_t i = 0; i < n; ++i)
		{
			int x, y;
			ss >> x >> y;
			pts.emplace_back(x, y);
		}
		return std::make_shared<ObjPolyLine>(a, pts);
	}

	return nullptr;
}
