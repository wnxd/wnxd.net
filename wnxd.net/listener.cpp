#include "listener.h"

using namespace wnxd::Listener;
using namespace System::Reflection;
//class ListenerMethod
//public
bool ListenerMethod::PreRun(Object^ target, array<Object^>^ args)
{
	return true;
}
bool ListenerMethod::EndRun(Object^ target, array<Object^>^ args, Object^ result)
{
	return true;
}
//class ListenerField
//public
bool ListenerField::PreGet(Object^ target, String^ fieldname)
{
	return true;
}
bool ListenerField::PreSet(Object^ target, String^ fieldname, Object^ value)
{
	return true;
}
bool ListenerField::EndGet(Object^ target, String^ fieldname, Object^ result)
{
	return true;
}
bool ListenerField::EndSet(Object^ target, String^ fieldname, Object^ value)
{
	return true;
}
//class ListenerProperty
//public
bool ListenerProperty::PreGet(Object^ target, String^ propertyname, array<Object^>^ param)
{
	return true;
}
bool ListenerProperty::PreSet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value)
{
	return true;
}
bool ListenerProperty::EndGet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ result)
{
	return true;
}
bool ListenerProperty::EndSet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value)
{
	return true;
}
//class MonitorAttribute
//private
IMessageSink^ ListenerAttribute::GetServerContextSink(IMessageSink^ nextSink)
{
	return gcnew Sink(this, nextSink);
}
IMessageSink^ ListenerAttribute::GetEnvoySink(MarshalByRefObject^ obj, IMessageSink^ nextSink)
{
	this->_this = obj;
	return nextSink;
}
//public
ListenerAttribute::ListenerAttribute() : ContextAttribute("wnxd.Monitor")
{

}
//class Sink
//private
bool ListenerAttribute::Sink::MethodPreRun(array<ListenerMethod^>^ list, array<Object^>^ args)
{
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->PreRun(this->_listener->_this, args)) return false;
	return true;
}
bool ListenerAttribute::Sink::MethodEndRun(array<ListenerMethod^>^ list, array<Object^>^ args, Object^ result)
{
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->EndRun(this->_listener->_this, args, result)) return false;
	return true;
}
bool  ListenerAttribute::Sink::FieldPreGet(array<ListenerField^>^ list, String^ fieldname)
{
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->PreGet(this->_listener->_this, fieldname)) return false;
	return true;
}
bool  ListenerAttribute::Sink::FieldPreSet(array<ListenerField^>^ list, String^ fieldname, Object^ value)
{
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->PreSet(this->_listener->_this, fieldname, value)) return false;
	return true;
}
bool  ListenerAttribute::Sink::FieldEndGet(array<ListenerField^>^ list, String^ fieldname, Object^ result)
{
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->EndGet(this->_listener->_this, fieldname, result)) return false;
	return true;
}
bool  ListenerAttribute::Sink::FieldEndSet(array<ListenerField^>^ list, String^ fieldname, Object^ value)
{
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->EndSet(this->_listener->_this, fieldname, value)) return false;
	return true;
}
bool ListenerAttribute::Sink::PropertyPreGet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param)
{
	if (propertyname == "Item" && param->Length > 0) propertyname = "this";
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->PreGet(this->_listener->_this, propertyname, param)) return false;
	return true;
}
bool ListenerAttribute::Sink::PropertyPreSet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ value)
{
	if (propertyname == "Item" && param->Length > 0) propertyname = "this";
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->PreSet(this->_listener->_this, propertyname, param, value)) return false;
	return true;
}
bool ListenerAttribute::Sink::PropertyEndGet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ result)
{
	if (propertyname == "Item" && param->Length > 0) propertyname = "this";
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->EndGet(this->_listener->_this, propertyname, param, result)) return false;
	return true;
}
bool ListenerAttribute::Sink::PropertyEndSet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ value)
{
	if (propertyname == "Item" && param->Length > 0) propertyname = "this";
	for (int i = list->Length - 1; i >= 0; i--) if (!list[i]->EndSet(this->_listener->_this, propertyname, param, value)) return false;
	return true;
}
IMessageSink^ ListenerAttribute::Sink::NextSink::get()
{
	return this->_nextSink;
}
IMessage^ ListenerAttribute::Sink::SyncProcessMessage(IMessage^ msg)
{
	String^ TypeName = (String^)msg->Properties["__TypeName"];
	Type^ T = Type::GetType(TypeName);
	if (T != nullptr)
	{
		IMessage^ retMsg;
		BindingFlags all = BindingFlags::Instance | BindingFlags::Public | BindingFlags::NonPublic;
		String^ MethodName = (String^)msg->Properties["__MethodName"];
		array<Type^>^ MethodSignature = (array<Type^>^)msg->Properties["__MethodSignature"];
		array<Object^>^ Args = (array<Object^>^)msg->Properties["__Args"];
		if (MethodName == ".ctor")
		{
			this->_typename = TypeName;
			ConstructorInfo^ ci = T->GetConstructor(all, nullptr, MethodSignature, nullptr);
			array<ListenerMethod^>^ list = (array<ListenerMethod^>^)ci->GetCustomAttributes(ListenerMethod::typeid, true);
			if (!this->MethodPreRun(list, Args)) return nullptr;
			retMsg = this->_nextSink->SyncProcessMessage(msg);
			if (!this->MethodEndRun(list, (array<Object^>^)retMsg->Properties["__OutArgs"], retMsg->Properties["__Return"])) return nullptr;
		}
		else if (MethodName == "FieldGetter" && TypeName != this->_typename)
		{
			T = Type::GetType(this->_typename);
			String^ fieldname = (String^)Args[1];
			FieldInfo^ fi = T->GetField(fieldname, all);
			if (!this->FieldPreGet((array<ListenerField^>^)fi->GetCustomAttributes(ListenerField::typeid, true), fieldname)) return nullptr;
			retMsg = this->_nextSink->SyncProcessMessage(msg);
			Args = (array<Object^>^)retMsg->Properties["__OutArgs"];
			if (!this->FieldEndGet((array<ListenerField^>^)fi->GetCustomAttributes(ListenerField::typeid, true), fieldname, Args[2])) return nullptr;
		}
		else if (MethodName == "FieldSetter" && TypeName != this->_typename)
		{
			T = Type::GetType(this->_typename);
			String^ fieldname = (String^)Args[1];
			FieldInfo^ fi = T->GetField(fieldname, all);
			if (!this->FieldPreSet((array<ListenerField^>^)fi->GetCustomAttributes(ListenerField::typeid, true), fieldname, Args[2])) return nullptr;
			retMsg = this->_nextSink->SyncProcessMessage(msg);
			if (!this->FieldEndSet((array<ListenerField^>^)fi->GetCustomAttributes(ListenerField::typeid, true), fieldname, Args[2])) return nullptr;
		}
		else
		{
			array<String^>^ arr = MethodName->Split('_');
			PropertyInfo^ pi;
			array<ListenerProperty^>^ plist;
			if (arr->Length == 2 && (arr[0] == "get" || arr[0] == "set") && (pi = T->GetProperty(arr[1], all)) != nullptr)
			{
				plist = (array<ListenerProperty^>^)pi->GetCustomAttributes(ListenerProperty::typeid, true);
				if (plist->Length > 0)
				{
					if (arr[0] == "get") if (!this->PropertyPreGet(plist, arr[1], Args)) return nullptr;
					else if (arr[0] == "set")
					{
						int len = Args->Length - 1;
						array<Object^>^ param = gcnew array<Object^>(len);
						if (len > 0) Array::Copy(Args, param, len);
						if (!this->PropertyPreSet(plist, arr[1], param, Args[len])) return nullptr;
					}
				}
			}
			MethodInfo^ mi = T->GetMethod(MethodName, all, nullptr, MethodSignature, nullptr);
			array<ListenerMethod^>^ mlist = (array<ListenerMethod^>^)mi->GetCustomAttributes(ListenerMethod::typeid, true);
			if (!this->MethodPreRun(mlist, Args)) return nullptr;
			retMsg = this->_nextSink->SyncProcessMessage(msg);
			Object^ result = retMsg->Properties["__Return"];
			Args = (array<Object^>^)retMsg->Properties["__OutArgs"];
			if (pi != nullptr)
			{
				if (plist->Length > 0)
				{
					if (arr[0] == "get") if (!this->PropertyEndGet(plist, arr[1], Args, result)) return nullptr;
					else if (arr[0] == "set")
					{
						int len = Args->Length - 1;
						array<Object^>^ param = gcnew array<Object^>(len);
						if (len > 0) Array::Copy(Args, param, len);
						if (!this->PropertyEndSet(plist, arr[1], param, Args[len])) return nullptr;
					}
				}
			}
			if (!this->MethodEndRun(mlist, Args, result)) return nullptr;
		}
		return retMsg;
	}
	return this->_nextSink->SyncProcessMessage(msg);
}
IMessageCtrl^ ListenerAttribute::Sink::AsyncProcessMessage(IMessage^ msg, IMessageSink^ replySink)
{
	return this->_nextSink->AsyncProcessMessage(msg, replySink);
}
//internal
ListenerAttribute::Sink::Sink(ListenerAttribute^ listener, IMessageSink^ nextSink)
{
	this->_listener = listener;
	this->_nextSink = nextSink;
}