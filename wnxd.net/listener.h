using namespace System;
using namespace System::Runtime::Remoting::Contexts;
using namespace System::Runtime::Remoting::Messaging;

namespace wnxd
{
	namespace Listener
	{
		/// <summary>
		/// 监听构造函数与实例方法的运行过程,待监听的类需要添加[Listener]属性
		/// </summary>
		[AttributeUsage(AttributeTargets::Constructor | AttributeTargets::Method, AllowMultiple = true, Inherited = true)]
		public ref class ListenerMethod abstract : Attribute
		{
		public:
			/// <summary>
			/// 方法运行前,返回false中断运行
			/// </summary>
			/// <param name="target">包含此方法的对象.当方法为构造函数时,此参数为null</param>
			/// <param name="args">所传参数</param>
			/// <returns></returns>
			virtual bool PreRun(Object^ target, array<Object^>^ args);
			/// <summary>
			/// 方法运行后,返回false中断运行
			/// </summary>
			/// <param name="target">包含此方法的对象</param>
			/// <param name="args">所传参数</param>
			/// <param name="result">方法的返回值</param>
			/// <returns></returns>
			virtual bool EndRun(Object^ target, array<Object^>^ args, Object^ result);
			/// <summary>
			/// 方法异常时,返回true表示处理了此异常
			/// </summary>
			/// <param name="target">包含此方法的对象.当方法为构造函数时,此参数为null</param>
			/// <param name="args">所传参数</param>
			/// <param name="ex"></param>
			/// <returns></returns>
			virtual bool Exception(Object^ target, array<Object^>^ args, Exception^ ex);
		};
		/// <summary>
		/// 监听变量的读取与修改事件,待监听的类需要添加[Listener]属性
		/// </summary>
		[AttributeUsage(AttributeTargets::Field, AllowMultiple = true, Inherited = true)]
		public ref class ListenerField abstract : Attribute
		{
		public:
			/// <summary>
			/// 变量取值前,返回false中断运行
			/// </summary>
			/// <param name="target">包含此变量的对象</param>
			/// <param name="fieldname">变量名</param>
			/// <returns></returns>
			virtual bool PreGet(Object^ target, String^ fieldname);
			/// <summary>
			/// 变量赋值前,返回false中断运行
			/// </summary>
			/// <param name="target">包含此变量的对象</param>
			/// <param name="fieldname">变量名</param>
			/// <param name="value">欲赋给变量的值</param>
			/// <returns></returns>
			virtual bool PreSet(Object^ target, String^ fieldname, Object^ value);
			/// <summary>
			/// 变量取值后,返回false中断运行
			/// </summary>
			/// <param name="target">包含此变量的对象</param>
			/// <param name="fieldname">变量名</param>
			/// <param name="result">当前变量的值</param>
			/// <returns></returns>
			virtual bool EndGet(Object^ target, String^ fieldname, Object^ result);
			/// <summary>
			/// 变量赋值后,返回false中断运行
			/// </summary>
			/// <param name="target">包含此变量的对象</param>
			/// <param name="fieldname">变量名</param>
			/// <param name="value">欲赋给变量的值</param>
			/// <returns></returns>
			virtual bool EndSet(Object^ target, String^ fieldname, Object^ value);
		};
		/// <summary>
		/// 监听属性和索引器的读取与修改事件,待监听的类需要添加[Listener]属性
		/// </summary>
		[AttributeUsage(AttributeTargets::Property, AllowMultiple = true, Inherited = true)]
		public ref class ListenerProperty abstract : Attribute
		{
		public:
			/// <summary>
			/// 属性取值前,返回false中断运行
			/// </summary>
			/// <param name="target">包含此属性的对象</param>
			/// <param name="propertyname">属性名</param>
			/// <param name="param">索引参数</param>
			/// <returns></returns>
			virtual bool PreGet(Object^ target, String^ propertyname, array<Object^>^ param);
			/// <summary>
			/// 属性赋值前,返回false中断运行
			/// </summary>
			/// <param name="target">包含此属性的对象</param>
			/// <param name="propertyname">属性名</param>
			/// <param name="param">索引参数</param>
			/// <param name="value">欲赋给属性的值</param>
			/// <returns></returns>
			virtual bool PreSet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value);
			/// <summary>
			/// 属性取值后,返回false中断运行
			/// </summary>
			/// <param name="target">包含此属性的对象</param>
			/// <param name="propertyname">属性名</param>
			/// <param name="param">索引参数</param>
			/// <param name="result">当前属性的值</param>
			/// <returns></returns>
			virtual bool EndGet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ result);
			/// <summary>
			/// 属性赋值后,返回false中断运行
			/// </summary>
			/// <param name="target">包含此属性的对象</param>
			/// <param name="propertyname">属性名</param>
			/// <param name="param">索引参数</param>
			/// <param name="value">欲赋给属性的值</param>
			/// <returns></returns>
			virtual bool EndSet(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value);
			/// <summary>
			/// 属性异常时,返回true表示处理了此异常
			/// </summary>
			/// <param name="target">包含此属性的对象</param>
			/// <param name="propertyname">属性名</param>
			/// <param name="param">索引参数</param>
			/// <param name="value">欲赋给属性的值</param>
			/// <param name="ex"></param>
			/// <returns></returns>
			virtual bool Exception(Object^ target, String^ propertyname, array<Object^>^ param, Object^ value, Exception^ ex);
		};
		/// <summary>
		/// 监听当前类的相关操作事件,需要待监听类继承ContextBoundObject
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