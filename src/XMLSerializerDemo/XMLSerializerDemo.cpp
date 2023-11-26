// XMLSerialize.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "XMLSerializerDemo.h"
#include <iostream>

#include "SerializerXml.h"
#include "SerializerRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//----------------------------------------------------------)

using namespace std;
using namespace Rsdn;
using namespace Serializer;
using namespace Tree;

//----------------------------------------------------------)

struct Clr
{
	Clr() : r(0), g(0), b(0) {}
	Clr(byte ar, byte ag, byte ab)
		: r(ar), g(ag), b(ab) {}

	byte r;
	byte g;
	byte b;

	const static Clr white;
	const static Clr black;

	struct LayoutDefault : public Layout<Clr>
	{
		LayoutDefault()
		{
			Attribute(_T("red"  ), &Clr::r, (byte)255);
			Attribute(_T("green"), &Clr::g, (byte)255);
			Attribute(_T("blue" ), &Clr::b, (byte)255);
		}
	};

	struct Layout2	: public Layout<Clr>
	{
		Layout2()
		{	
			Simple(_T("Red"  ), &Clr::r, (byte)0);
			Simple(_T("Green"), &Clr::g, (byte)0);
			Simple(_T("Blue" ), &Clr::b, (byte)0);
		}
	};
};
//----------------------------------------------------------)

const Clr Clr::white = Clr(255, 255, 255);
const Clr Clr::black = Clr(0, 0, 0);

//----------------------------------------------------------)

const StructClassMeta<Clr>& Clr2_ClassMeta()
{
	static Clr::Layout2       layout;
	static StructClassMeta<Clr> metaData(layout);
	return metaData;
}
//----------------------------------------------------------)

struct Clr_Layout3	: public Layout<Clr>
{
	Clr_Layout3()
	{	
		Attribute(_T("Red"  ), &Clr::r, (byte)0);
		Attribute(_T("Green"), &Clr::g, (byte)0);
		Attribute(_T("Blue" ), &Clr::b, (byte)0);
	}
};
//----------------------------------------------------------)

const StructClassMeta<Clr>& Clr3_ClassMeta()
{
	static Clr_Layout3        layout;
	static StructClassMeta<Clr> metaData(layout);
	return metaData;
}
//----------------------------------------------------------)

enum DayOfWeek
{
	Sunday, Monday, Tuesday, Wednesday, Thursday, 
	Friday, Saturday 
};

struct DayOfWeek_EnumMetaData 
	 : public EnumMetaData<DayOfWeek>
{
	DayOfWeek_EnumMetaData()
	{
		Enumerator(_T("Sunday"),    Sunday   );
		Enumerator(_T("Monday"),    Monday   );
		Enumerator(_T("Tuesday"),   Tuesday  );
		Enumerator(_T("Wednesday"), Wednesday);
		Enumerator(_T("Thursday"),  Thursday );
		Enumerator(_T("Friday"),    Friday   );
		Enumerator(_T("Saturday"),  Saturday );
	}
};

ENUM_METADATA(DayOfWeek, DayOfWeek_EnumMetaData);
//----------------------------------------------------------)

struct Pnt2
{
	int               x;
	double            y;
	std::vector<long> vec;
	Clr               color;
	std::vector<Clr>  colors;
	std::vector<Clr>  colors2;
	std::vector<Clr>  colors3;
	DayOfWeek         dow;  

	struct LayoutDefault : public Layout<Pnt2>
	{
		LayoutDefault()
		{
			Simple (_T("x"      ), &Pnt2::x,       5           );
			Simple (_T("y"      ), &Pnt2::y,       7.0         );
			Complex(_T("color"  ), &Pnt2::color  , Clr::white);
			Array  (_T("vec"    ), &Pnt2::vec    , _T("long"  ));
			Array  (_T("colors" ), &Pnt2::colors , _T("color" ));
			Array  (_T("Colors" ), &Pnt2::colors2, _T("Color"  ), Clr2_ClassMeta());
			Array  (_T("Clrs"   ), &Pnt2::colors3, _T("Clr"    ), Clr3_ClassMeta());
			Simple (_T("DayOfWeek" ), &Pnt2::dow , Sunday);
		}
	};
};
//----------------------------------------------------------)

struct Pnt
{
	int               x;
	double            y;
	std::vector<long> vec;
	Clr               color;

	struct LayoutDefault : public Layout<Pnt>
	{
		LayoutDefault()
		{
			Simple   (_T("x"      ), &Pnt::x,       5          );
			Attribute(_T("y"      ), &Pnt::y,       7.0        );
			Complex  (_T("color"  ), &Pnt::color  , Clr::white);
			Array    (_T("vec"    ), &Pnt::vec    , _T("item" ));
		}
	};
};
//----------------------------------------------------------)

struct Day
{
	Day(){}
	Day(DayOfWeek a_dow, bool a_holiday)
		: dow(a_dow), holiday(a_holiday)
	{}
	bool      holiday;
	DayOfWeek dow;  
	const static bool defaultHoliday = true;

	struct LayoutDefault : public Layout<Day>
	{
		LayoutDefault()
		{
			Simple (_T("Holiday"),    &Day::holiday, true  );
			Simple (_T("DayOfWeek" ), &Day::dow,     Sunday);
		}
	};
};
//----------------------------------------------------------)

namespace Rsdn
{
namespace dotNet
{
struct Pair
{
	tstring key, value;

	struct LayoutDefault : public Layout<Pair>
	{
		LayoutDefault()
		{
			AttributeM(_T("key"  ), &Pair::key  );
			AttributeM(_T("value"), &Pair::value);
		}
	};
};

struct File
{
	typedef std::vector<Pair> Pairs;
	std::vector<Pair> pairs;

	struct LayoutDefault : public Layout<File>
	{
		LayoutDefault()
		{
			Array(_T("appSettings" ), &File::pairs, _T("add" ));
		}
	};
};

struct AppSettings
{
	typedef std::map<tstring, tstring> Values;

	AppSettings(){}
	AppSettings(const tstring& path)
	{
		Load(path);
	}

	void Load(const tstring& path)
	{
		File file;
		Xml::Load(path, _T("configuration"), &file);
		for_each(file.pairs.begin(), file.pairs.end()
			, std::bind1st(std::mem_fun1<void, AppSettings, Pair>(Add)
				, this));
	}

	tstring GetValue(const tstring& key, const tstring& def = _T("")) const
	{
		Values::const_iterator iter = m_values.find(key);
		return iter == m_values.end() ? def : iter->second;
	}

	void Add(Pair pair)
	{
		m_values.insert(Values::value_type(pair.key, pair.value));
	}
//protected:
	Values m_values;
};
//----------------------------------------------------------)
//----------------------------------------------------------)

namespace Config 
{

template <typename AppSettingsType>
struct ConfigAttribute : public CRefcounted
{
	virtual void ReadValue(AppSettingsType*) = 0;
};

template <typename AppSettingsType, typename ValueType>
struct ConfigValueAttribute
	: public ConfigAttribute<AppSettingsType>
{
	ConfigValueAttribute(const tstring& key
		, ValueType AppSettingsType::*    offset
		, ValueType                       def
		)
		: m_key(key), m_offset(offset)
		, m_default(def), m_mondatory(false)
	{}

	ConfigValueAttribute(const tstring& key
		, ValueType AppSettingsType::*    offset
		)
		: m_key(key), m_offset(offset)
		, m_default(), m_mondatory(true)
	{}

	void ReadValue(AppSettingsType* pAppSettings)
	{
		dotNet::AppSettings::Values::const_iterator iter = pAppSettings->m_values.find(m_key);
		if (iter != pAppSettings->m_values.end())
			pAppSettings->*m_offset = Primitive<ValueType>::Parse(iter->second);
		else
		{	if(m_mondatory)
				throw MondatoryFieldException(m_key);
			pAppSettings->*m_offset = m_default;
		}
	}
private:
	const tstring                      m_key;
	ValueType AppSettingsType::* const m_offset;
	const bool                         m_mondatory;
	ValueType                          m_default;
};

template <typename AppSettingsType>
struct Layout : std::vector<CPtrShared<ConfigAttribute<AppSettingsType> > >
{
	template <typename ValueType>
	void Value(const tstring& key, ValueType AppSettingsType::* offset, ValueType def)
	{
		push_back(new ConfigValueAttribute<AppSettingsType, ValueType>(key, offset, def));
	}

	template <typename ValueType>
	void ValueM(const tstring& key, ValueType AppSettingsType::* offset)
	{
		push_back(new ConfigValueAttribute<AppSettingsType, ValueType>(key, offset));
	}

	void Read(AppSettingsType* pAppSettings)
	{
		for_each(begin(), end(), ValueReader(pAppSettings));
	}

protected:
	struct ValueReader
	{
		ValueReader(AppSettingsType* pAppSettings) 
			: m_pAppSettings(pAppSettings) 
			{}
		void operator() (ConfigAttribute<AppSettingsType>* pField) 
			{ pField->ReadValue(m_pAppSettings);}
		AppSettingsType* m_pAppSettings;
	}; 
};

template <typename AppSettingsType>
void Load(const tstring& path, AppSettingsType* pAppSettings)
{
	pAppSettings->Load(path);
	static AppSettingsType::LayoutDefault g_layout;
	g_layout.Read(pAppSettings);	
}

} // namespace Config 
} // namespace dotNet
} // namespace Rsdn

struct MyConfig : public dotNet::AppSettings
{
	int x;
	double y;

	struct LayoutDefault : public dotNet::Config::Layout<MyConfig>
	{
		LayoutDefault()
		{
			Value(_T("x"), &MyConfig::x, 5  );
			Value(_T("y"), &MyConfig::y, 7.0);
		}
	};
};
//----------------------------------------------------------)

void Test()
{
	try
	{
		// Test 1 ///////////////////////////////

		int n;
		Xml::Load(_T("n.xml"),  _T("n"), &n);
		Xml::Save(_T("_n.xml"), _T("_n"), n);

		Registry::Save(HKEY_CURRENT_USER, _T("Software\\Rsdn\\XmlCpp\\n"), n);
		Registry::Load(HKEY_CURRENT_USER, _T("Software\\Rsdn\\XmlCpp\\n"), &n);

		tcout << _T("Test 1")  << endl
			  << _T("n=") << n << endl 
		      << _T("-----")   << endl 
			                   << endl; 

		// Test 2 ///////////////////////////////

		std::vector<int> vec;
		Xml::Load(_T("vec.xml"),  _T("vec"), &vec);
		Xml::Save(_T("_vec.xml"), _T("_vec"), vec);

		Registry::Save(HKEY_CURRENT_USER, _T("Software\\Rsdn\\XmlCpp\\vec"),  vec);
		Registry::Load(HKEY_CURRENT_USER, _T("Software\\Rsdn\\XmlCpp\\vec"), &vec);

		tcout << _T("Test 2") << endl << _T("vec.size=") << (int)vec.size();
		for (std::vector<int>::const_iterator pN = vec.begin(); pN != vec.end(); pN++)
			tcout << _T("vec[")  << (int)(pN - vec.begin()) << _T("]=") << *pN << endl;
		tcout << endl << _T("-----") << endl << endl;

		// Test 3 ///////////////////////////////

		Pnt2 pnt;
		Xml::Load(_T("struct.xml"),  _T("struct"), &pnt);
		Xml::Save(_T("_struct.xml"), _T("_struct"), pnt);

		Registry::Save(HKEY_CURRENT_USER, _T("Software\\Rsdn\\XmlCpp\\Pnt"),  pnt);
		Registry::Load(HKEY_CURRENT_USER, _T("Software\\Rsdn\\XmlCpp\\Pnt"), &pnt);


		tcout << _T("Test 3") << endl
		      << _T("pnt.x=") << pnt.x << _T(", pnt.y=") << pnt.y  << endl
		      << _T("pnt.color= (") << pnt.color.r 
			  << _T(", ") << pnt.color.g 
		      << _T(", ") << pnt.color.b  << _T(").") << endl;

		if (pnt.vec.size() == 0)
			tcout << _T("pnt.vec empty.") << endl;
		else
			for (std::vector<long>::const_iterator pN = pnt.vec.begin(); pN != pnt.vec.end(); pN++)
				tcout << _T("pnt.vec[")  << (int)(pN - pnt.vec.begin()) << _T("]=") << *pN << endl;

		if (pnt.colors.size() == 0)
			tcout << _T("pnt.colors empty.") << endl;
		else
			for (std::vector<Clr>::const_iterator pColor = pnt.colors.begin()
				; pColor != pnt.colors.end(); pColor++)
			{
		      tcout << _T("pnt.m_colors[") << (int)(pColor - pnt.colors.begin()) 
				  << _T("]= (") 
				  <<  pColor->r << _T(", ") << pColor->g << _T(", ") << pColor->b  
				  << _T(").") << endl;
			}

		if (pnt.colors2.size() == 0)
			tcout << _T("pnt.colors2 empty\n");
		else
			for (std::vector<Clr>::const_iterator pColor = pnt.colors2.begin()
				; pColor != pnt.colors2.end(); pColor++)
			{
				_tprintf(_T("pnt.m_colors2[%d]= (%d, %d, %d).\n")
				        , pColor - pnt.colors2.begin()
				        , pColor->r, pColor->g, pColor->b);
			}

		if (pnt.colors3.size() == 0)
			tcout << _T("pnt.colors3 empty\n");
		else
			for (std::vector<Clr>::const_iterator pColor = pnt.colors3.begin()
				; pColor != pnt.colors3.end(); pColor++)
			{
				_tprintf(_T("pnt.m_colors3[%d]= (%d, %d, %d).\n")
				        , pColor - pnt.colors3.begin()
				        , pColor->r, pColor->g, pColor->b);
			}

		tcout << endl << _T("-----") << endl << endl;

		// Test 4 ///////////////////////////////

		std::vector<Pnt2> vecPnt;
		Xml::Load(_T("vecStruct.xml"),  _T("vecStruct"), &vecPnt, DefaultVectorMetaClass<Pnt2>(_T("struct")));
		Xml::Save(_T("_vecStruct.xml"), _T("_vecStruct"), vecPnt, DefaultVectorMetaClass<Pnt2>(_T("struct")));

		Registry::Save(HKEY_CURRENT_USER, _T("Software\\Rsdn\\XmlCpp\\vecPnt"),  vecPnt, DefaultVectorMetaClass<Pnt2>(_T("struct")));
		Registry::Load(HKEY_CURRENT_USER, _T("Software\\Rsdn\\XmlCpp\\vecPnt"), &vecPnt, DefaultVectorMetaClass<Pnt2>(_T("struct")));

		for (std::vector<Pnt2>::const_iterator iter = vecPnt.begin()
			; iter != vecPnt.end(); iter++)
		{
			Pnt2 pnt = *iter;

			_tprintf(_T("Test 4 OK\npnt.x=%d, pnt.y=%g.\n"), pnt.x, pnt.y);
			_tprintf(_T("pnt.color= (%d, %d, %d).\n"), pnt.color.r, pnt.color.g, pnt.color.b);

			if (pnt.vec.size() == 0)
				_tprintf(_T("pnt.vec empty.\n"));
			else
				for (std::vector<long>::const_iterator pN = pnt.vec.begin(); pN != pnt.vec.end(); pN++)
					_tprintf(_T("pnt.vec[%d]=%d.\n"), pN - pnt.vec.begin(), *pN);

			if (pnt.colors.size() == 0)
				_tprintf(_T("pnt.colors empty.\n"));
			else
				for (std::vector<Clr>::const_iterator pColor = pnt.colors.begin()
					; pColor != pnt.colors.end(); pColor++)
				{
					_tprintf(_T("pnt.m_colors[%d]= (%d, %d, %d).\n")
					, pColor - pnt.colors.begin()
					, pColor->r, pColor->g, pColor->b);
				}

			if (pnt.colors2.size() == 0)
				_tprintf(_T("pnt.colors2 empty\n"));
			else
				for (std::vector<Clr>::const_iterator pColor = pnt.colors2.begin()
					; pColor != pnt.colors2.end(); pColor++)
				{
					_tprintf(_T("pnt.m_colors2[%d]= (%d, %d, %d).\n")
							, pColor - pnt.colors2.begin()
							, pColor->r, pColor->g, pColor->b);
				}

			if (pnt.colors3.size() == 0)
				_tprintf(_T("pnt.colors3 empty\n"));
			else
				for (std::vector<Clr>::const_iterator pColor = pnt.colors3.begin()
					; pColor != pnt.colors3.end(); pColor++)
				{
					_tprintf(_T("pnt.m_colors3[%d]= (%d, %d, %d).\n")
							, pColor - pnt.colors3.begin()
							, pColor->r, pColor->g, pColor->b);
				}
		}
		tcout << endl << _T("-----") << endl << endl;

		// Test 5 ///////////////////////////////

		dotNet::AppSettings settings(_T("app.exe.config"));

		tcout << _T("Test 5") << endl
			  << _T("TimeoutMillisec=") << settings.GetValue(_T("TimeoutMillisec")).c_str()
			  << _T("MaxCount=")        << settings.GetValue(_T("MaxCount")).c_str()<< endl 
			  << _T("-----")  << endl 
			                  << endl;

		// Test 6 ///////////////////////////////

		MyConfig config;
		dotNet::Config::Load(_T("app2.exe.config"), &config);

		tcout << _T("Test 6")      << endl
		      << _T("config.x=")   << config.x 
		      << _T(", config.y=") << config.y << endl
			  << _T("-----")       << endl 
			                       << endl;

		////////////////////////////
	}
	catch (Exception ex)
	{
		printf(ex.what());
	}
	catch(...)
	{
		printf("Unexpected error");
	}
}

CWinApp theApp;
using namespace std;

int _tmain(  int       // argc
		   //, TCHAR* [] // argv
		   //, TCHAR* [] // envp
		   )
{
	int nRetCode = 0;
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		tcout << _T("Fatal Error: MFC initialization failed\n");
		nRetCode = 1;
	}
	else
	{
		::CoInitialize(NULL);
		Test();
		::CoUninitialize();
		tcout << _T("\n\nPress any key to exit.\n");
		getch();
	}

	return nRetCode;
}
