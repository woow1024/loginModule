#pragma once

#define DECLARE_PROPERTY(property_name, property_type)	\
public:	\
	inline property_type property_name() const { return m_##property_name; }	\
	inline void Set##property_name(property_type value) { m_##property_name = value; }	\
private:	\
	property_type m_##property_name;


#define DECLARE_READONLY_PROPERTY(property_name, property_type)	\
public:	\
	inline property_type property_name() const { return m_##property_name; }	\
private:	\
	property_type m_##property_name;

#define DECLARE_WIRTEONLY_PROPERTY(property_name, property_type)	\
public:	\
	inline void Set##property_name(property_type value) { m_##property_name = value; }	\
private:	\
	property_type m_##property_name;

#define DECLARE_PURE_VIRTUAL_FUNC(ret_type, func_name, ...)	\
	virtual ret_type func_name(__VA_ARGS__) PURE;

#define DECLARE_CONST_PURE_VIRTUAL_FUNC(ret_type, func_name, ...)	\
	virtual ret_type func_name(__VA_ARGS__) const PURE;

#define DECLARE_VIRTUAL_FUNC(ret_type, func_name, ...)	\
	virtual ret_type func_name(__VA_ARGS__);

#define DECLARE_CONST_VIRTUAL_FUNC(ret_type, func_name, ...)	\
	virtual ret_type func_name(__VA_ARGS__) const;

#define OVERRIDE_FUNC(ret_type, func_name, ...)			 DECLARE_VIRTUAL_FUNC(ret_type, func_name, __VA_ARGS__)
#define OVERRIDE_CONST_FUNC(ret_type, func_name, ...)	 DECLARE_CONST_VIRTUAL_FUNC(ret_type, func_name, __VA_ARGS__)

#define DECLARE_FUNC(ret_type, func_name, ...)	\
	ret_type func_name(__VA_ARGS__);

#define DECLARE_CONST_FUNC(ret_type, func_name, ...)	\
	ret_type func_name(__VA_ARGS__) const;