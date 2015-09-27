using namespace System;
using namespace System::Runtime::Remoting::Contexts;
using namespace System::Runtime::Remoting::Messaging;

namespace wnxd
{
	namespace Listener
	{
		/// <summary>
		/// �������캯����ʵ�����������й���,������������Ҫ���[Listener]����
		/// </summary>
		[AttributeUsage(AttributeTargets::Constructor | AttributeTargets::Method, AllowMultiple = true, Inherited = true)]
		public ref class ListenerMethod abstract : Attribute
		{
		public:
			/// <summary>
			/// ��������ǰ,����false�ж�����
			/// </summary>
			/// <param name="target">�����˷����Ķ���.������Ϊ���캯��ʱ,�˲���Ϊnull</param>
			/// <param name="args">��������</param>
			/// <returns></returns>
			virtual bool PreRun(Object^ target, array<Object^>^ args);
			/// <summary>
			/// �������к�,����false�ж�����
			/// </summary>
			/// <param name="target">�����˷����Ķ���</param>
			/// <param name="args">��������</param>
			/// <param name="result">�����ķ���ֵ</param>
			/// <returns></returns>
			virtual bool EndRun(Object^ target, array<Object^>^ args, Object^ result);
			/// <summary>
			/// �����쳣ʱ,����true��ʾ�����˴��쳣
			/// </summary>
			/// <param name="target">�����˷����Ķ���.������Ϊ���캯��ʱ,�˲���Ϊnull</param>
			/// <param name="args">��������</param>
			/// <param name="ex"></param>
			/// <returns></returns>
			virtual bool Exception(Object^ target, array<Object^>^ args, Exception^ ex);
		};
		/// <summary>
		/// ���������Ķ�ȡ���޸��¼�,������������Ҫ���[Listener]����
		/// </summary>
		[AttributeUsage(AttributeTargets::Field, AllowMultiple = true, Inherited = true)]
		public ref class ListenerField abstract : Attribute
		{
		public:
			/// <summary>
			/// ����ȡֵǰ,����false�ж�����
			/// </summary>
			/// <param name="target">�����˱����Ķ���</param>
			/// <param name="fieldname">������</param>
			/// <returns></returns>
			virtual bool PreGet(Object^ target, String^ fieldname);
			/// <summary>
			/// ������ֵǰ,����false�ж�����
			/// </summary>
			/// <param name="target">�����˱����Ķ���</param>
			/// <param name="fieldname">������</param>
			/// <param name="value">������������ֵ</param>
			/// <returns></returns>
			virtual bool PreSet(Object^ target, String^ fieldname, Object^ value);
			/// <summary>
			/// ����ȡֵ��,����false�ж�����
			/// </summary>
			/// <param name="target">�����˱����Ķ���</param>
			/// <param name="fieldname">������</param>
			/// <param name="result">��ǰ������ֵ</param>
			/// <returns></returns>
			virtual bool EndGet(Object^ target, String^ fieldname, Object^ result);
			/// <summary>
			/// ������ֵ��,����false�ж�����
			/// </summary>
			/// <param name="target">�����˱����Ķ���</param>
			/// <param name="fieldname">������</param>
			/// <param name="value">������������ֵ</param>
			/// <returns></returns>
			virtual bool EndSet(Object^ target, String^ fieldname, Object^ value);
		};
		/// <summary>
		/// �������Ժ��������Ķ�ȡ���޸��¼�,������������Ҫ���[Listener]����
		/// </summary>
		[AttributeUsage(AttributeTargets::Property, AllowMultiple = true, Inherited = true)]
		public ref class ListenerProperty abstract : Attribute
		{
		public:
			/// <summary>
			/// ����ȡֵǰ,����false�ж�����
			/// </summary>
			/// <param name="target">���������ԵĶ���</param>
			/// <param name="propertyname">������</param>
			/// <param name="param">��������</param>
			/// <returns></returns>
			virtual bool PreGet(Object^ target, String^ propertyname, array<Object^>^ param);
			/// <summary>
			/// ���Ը�ֵǰ,����false�ж�����
			/// </summary>
			/// <param name="target">���������ԵĶ���</param>
			/// <param name="propertyname">������</param>
			/// <param name="param">��������</param>
			/// <param name="value">���������Ե�ֵ</param>
			/// <returns></returns>
			virtual bool PreSet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value);
			/// <summary>
			/// ����ȡֵ��,����false�ж�����
			/// </summary>
			/// <param name="target">���������ԵĶ���</param>
			/// <param name="propertyname">������</param>
			/// <param name="param">��������</param>
			/// <param name="result">��ǰ���Ե�ֵ</param>
			/// <returns></returns>
			virtual bool EndGet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ result);
			/// <summary>
			/// ���Ը�ֵ��,����false�ж�����
			/// </summary>
			/// <param name="target">���������ԵĶ���</param>
			/// <param name="propertyname">������</param>
			/// <param name="param">��������</param>
			/// <param name="value">���������Ե�ֵ</param>
			/// <returns></returns>
			virtual bool EndSet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value);
			/// <summary>
			/// �����쳣ʱ,����true��ʾ�����˴��쳣
			/// </summary>
			/// <param name="target">���������ԵĶ���</param>
			/// <param name="propertyname">������</param>
			/// <param name="param">��������</param>
			/// <param name="value">���������Ե�ֵ</param>
			/// <param name="ex"></param>
			/// <returns></returns>
			virtual bool Exception(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value, Exception^ ex);
		};
		/// <summary>
		/// ������ǰ�����ز����¼�,��Ҫ��������̳�ContextBoundObject
		/// </summary>
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
				bool MethodException(array<ListenerMethod^>^ list, array<Object^>^ args, Exception^ ex);
				bool FieldPreGet(array<ListenerField^>^ list, String^ fieldname);
				bool FieldPreSet(array<ListenerField^>^ list, String^ fieldname, Object^ value);
				bool FieldEndGet(array<ListenerField^>^ list, String^ fieldname, Object^ result);
				bool FieldEndSet(array<ListenerField^>^ list, String^ fieldname, Object^ value);
				bool PropertyPreGet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param);
				bool PropertyPreSet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ value);
				bool PropertyEndGet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ result);
				bool PropertyEndSet(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ value);
				bool PropertyException(array<ListenerProperty^>^ list, String^ propertyname, array<Object^>^ param, Object^ value, Exception^ ex);
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