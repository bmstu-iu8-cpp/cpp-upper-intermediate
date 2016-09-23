#include <typeinfo>

#include <algorithm>
#include <exception>

class any
{
public:
	~any()
	{
		delete data;
	}

	any() : data(nullptr)
	{ }

	any(const any & rhs)
		: data((rhs.data == nullptr) ? nullptr : rhs.data->clone())
	{ }

	template<class T>
	any(T v) :
		data(new Holder<T>(v))
	{ }

	any(any && rhs)
		: data(rhs.data)
	{
		rhs.data = nullptr;
	}

	any & operator=(const any & rhs)
	{
		any(rhs).swap(*this);
	}

	template<class T>
	any & operator= (T value)
	{
		any(value).swap(*this);
		return *this;
	}

	any & swap(any & rhs)
	{
		std::swap(this->data, rhs.data);
		return *this;
	}

	struct IPresenter
	{
		virtual IPresenter * clone() const = 0;
		virtual const std::type_info& type() = 0;
		virtual ~IPresenter() {}
	};

	template<class Type>
	struct Holder : public IPresenter
	{
		Holder(Type v) : value(v)
		{ }

		virtual IPresenter * clone() const override
		{
			return new Holder(value);
		}

		virtual const std::type_info& type() override
		{
			return typeid(value);
		}

		Type value;
	};

//private:
	IPresenter * data;
};

template<class T>
T any_cast(const any & obj)
{
	return static_cast<any::Holder<T> *>(obj.data)->value;
}
