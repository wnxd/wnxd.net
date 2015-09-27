using namespace System;
using namespace System::Runtime::Remoting::Contexts;
using namespace System::Runtime::Remoting::Messaging;

namespace wnxd
{
	namespace Listener
	{
		[AttributeUsage(AttributeTargets::Constructor | AttributeTargets::Method, AllowMultiple = true, Inherited = true)]
		public ref class ListenerMethod abstract : Attribute
		{
		public:
			virtual bool PreRun(Object^ target, array<Object^>^ args);
			virtual bool EndRun(Object^ target, array<Object^>^ args, Object^ result);
		};
		[AttributeUsage(AttributeTargets::Field, AllowMultiple = true, Inherited = true)]
		public ref class ListenerField abstract : Attribute
		{
		public:
			virtual bool PreGet(Object^ target, String^ fieldname);
			virtual bool PreSet(Object^ target, String^ fieldname, Object^ value);
			virtual bool EndGet(Object^ target, String^ fieldname, Object^ result);
			virtual bool EndSet(Object^ target, String^ fieldname, Object^ value);
		};
		[AttributeUsage(AttributeTargets::Property, AllowMultiple = true, Inherited = true)]
		public ref class ListenerProperty abstract : Attribute
		{
		public:
			virtual bool PreGet(Object^ target, String^ propertyname, array<Object^>^ param);
			virtual bool PreSet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value);
			virtual bool EndGet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ result);
			virtual bool EndSet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value);
		};
		
		[AttributeUsage(AttributeTargets::Class, AllowMultiple = false, Inherited = false)]
		public ref class ListenerAttribute sealed : ContextAttribute, IContributeServerContextSink, IContributeEnvoySink
		{
		private:
			MarshalByRefObject^ _this;
			ref class Sink : IMessageSink
			{
			private:
				ListenerAttribute^ _listener;
				IMessageSink^ _nextSink;
				String^ _typename;
				bool MethodPreRun(array<ListenerMethod^>^ list, array<Object^>^ args);
				bool MethodEndRun(array<ListenerMethod^>^ list, array<Object^>^ args, Object^ result);
				bool FieldPreGet(array<ListenerField^>^ list, String^ fieldname);
				bool FieldPreSet(array<ListenerField^>^ list, String^ fieldname, Object^ value);
				bool FieldEndGet(array<ListenerField^>^ list, String^ fieldname, Object^ result);
				bool FieldEndSet(array<ListenerField^>^ list, String^ fieldname, Object^ value);
				bool PropertyPreGet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param);
				bool PropertyPreSet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ value);
				bool PropertyEndGet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ result);
				bool PropertyEndSet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ value);
				property IMessageSink^ NextSink
				{
					virtual IMessageSink^ get() sealed = IMessageSink::NextSink::get;
				}
				virtual IMessage^ SyncProcessMessage(IMessage^ msg) sealed = IMessageSink::SyncProcessMessage;
				virtual IMessageCtrl^ AsyncProcessMessage(IMessage^ msg, IMessageSink^ replySink) sealed = IMessageSink::AsyncProcessMessage;
			internal:
				Sink(ListenerAttribute^ listener, IMessageSink^ nextSink);
			};
			virtual IMessageSink^ GetServerContextSink(IMessageSink^ nextSink) sealed = IContributeServerContextSink::GetServerContextSink;
			virtual IMessageSink^ GetEnvoySink(MarshalByRefObject^ obj, IMessageSink^ nextSink) sealed = IContributeEnvoySink::GetEnvoySink;
		public:
			ListenerAttribute();
		};
	}
}