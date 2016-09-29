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
		return *this;
	}


	any & operator=(any && rhs)
	{
		any(std::move(rhs)).swap(*this);
		return *this;
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

	bool has_value() const
	{
		return data == nullptr;
	}

	void reset()
	{
		any().swap(*this);
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

private:
	IPresenter * data;

	template<class T>
	friend T * any_cast(any * op);
};

class bad_any_cast :
	public std::bad_cast
{
public:
	virtual const char * what() const override
	{
		return "bad_any_cast exception";
	}
};


template<class T>
T * any_cast(any * op)
{
	bool isSame = op->data->type() == typeid(T);
	return op && isSame ?
		&(static_cast<any::Holder<T> *>(op->data)->value) :
		nullptr;
}

template<class T>
T * any_cast(const any * op)
{
	return any_cast<T>(const_cast<any*>(op));
}


template<class T>
inline T any_cast(const any & op)
{
	auto ptr = any_cast<T>(&op);

	if (ptr == nullptr)
		throw bad_any_cast();

	return *ptr;
}