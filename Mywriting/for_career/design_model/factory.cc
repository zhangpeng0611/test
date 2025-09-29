/*

模式	         核心思想	                工厂数量	           产品关系	              扩展性（新增产品）	         适用场景
简单工厂	单一工厂创建所有产品	        1 个工厂	            无关联	              需修改工厂（差）	           产品少且稳定
工厂方法	每个产品对应一个工厂	        N 个工厂（1:1）	        无关联	              新增工厂即可（好）	      产品多且需频繁扩展
抽象工厂	每个工厂创建一整套相关产品族	N 个工厂（1:N）	      产品族内相关联	      扩展产品族困难	        需要一系列相关产品的场景

*/



#if 0
#include <iostream>
using namespace std;

//简单工厂模式

//定义产品的父类
class productparent
{
public:
	virtual void transform() = 0;
	virtual void ability() = 0;
	virtual ~productparent() {};
};

//定义产品具体子类
class productlion :public productparent
{
public:
	void transform() override
	{
		cout << "变成狮子形态" << endl;
	}
	void ability() override
	{
		cout << "拥有狮吼功的技能" << endl;
	}
};

class productbat :public productparent
{
public:
	void transform() override
	{
		cout << "变成了蝙蝠侠" << endl;
	}
	void ability() override
	{
		cout << "拥有看吸血的技能" << endl;
	}
};


//定义生产产品的工厂
enum class type :char { lion, bat };
class Factory
{
public:
	productparent* creatproduct(type t)
	{
		productparent* obj = nullptr;
		switch (t)
		{
		case type::lion:
			obj = new productlion;
			break;
		case type::bat:
			obj = new productbat;
			break;
		default:
			break;
		}
		return obj;
	}
};


int main()
{
	Factory* f = new Factory();
	f->creatproduct(type::lion)->transform();
	f->creatproduct(type::lion)->ability();

	return 0;
}



#endif





#if 0
//工厂模式

#include <iostream>
using namespace std;


//定义产品父类
class productparent
{
public:
	virtual void transform() = 0;
	virtual void ability() = 0;
	virtual ~productparent() {}
};

//定义产品子类
class produtlion :public productparent
{
public:
	void transform() override
	{
		cout << "变成狮子形态" << endl;
	}
	void ability() override
	{
		cout << "拥有狮吼功的技能" << endl;
	}
};

class productbat :public productparent
{
public:
	void transform() override
	{
		cout << "变成蝙蝠形态" << endl;
	}
	void ability() override
	{
		cout << "拥有吸血的技能" << endl;
	}
};


//定义生产产品的工厂
class Factory
{
public:
	virtual productparent* creatpro() = 0;
	virtual ~Factory(){}
};

//定义子类工厂
class lionfactory:public Factory
{
public:
	productparent* creatpro() override
	{
		return new produtlion;
	}
};

class batfactory :public Factory
{
public:
	productparent* creatpro() override
	{
		return new productbat;
	}
};

int main()
{
	Factory* f = new lionfactory;
	f->creatpro()->transform();
	f->creatpro()->ability();


	return 0;
}

#endif


#if 0

#include <iostream>
using namespace std;

/*
      抽象工厂模式

	  解释抽象工厂本质：
										   船
			   船体（抽象类）       武器（抽象类）          引擎（抽象类）
		   1      2        3       1      2     3         1        2        3
*/


//船体
class shipbody
{
public:
	virtual void getbody() = 0;
	virtual ~shipbody() {}
};

class woodenbody :public shipbody
{
public:
	void getbody() override
	{
		cout << "船体材料是木头" << endl;
	}
};

class steelbody :public shipbody
{
public:
	void getbody() override
	{
		cout << "船体材料是钢铁" << endl;
	}
};

//武器类
class weapon
{
public:
	virtual void getweapon() = 0;
	virtual ~weapon() {}

};

class knife :public weapon
{
public:
	void getweapon() override
	{
		cout << "武器是刀" << endl;
	}
};

class gun :public weapon
{
public:
	void getweapon() override
	{
		cout << "武器是枪" << endl;
	}
};

//引擎
class engin
{
public:
	virtual void getengin() = 0;
	virtual ~engin() {}
};

class manual :public engin
{
	void getengin()
	{
		cout << "引擎是手动" << endl;
	}
};

class air :public engin
{
	void getengin()
	{
		cout << "引擎是气动" << endl;
	}
};

//船
class ship
{
public:
	ship(shipbody* body, weapon* weapon, engin* engin) :
		m_body(body), m_engin(engin), m_weapon(weapon)
	{
		cout << "该船被组装起来了" << endl;
	}
private:
	shipbody* m_body;
	engin* m_engin;
	weapon* m_weapon;
};


int main()
{

	shipbody* body = new steelbody;
	weapon* w = new gun;
	engin* eng = new air;
	ship* s = new ship(body, w, eng);

}


#endif