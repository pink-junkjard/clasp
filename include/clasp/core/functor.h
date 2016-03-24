#ifndef functor_h
#define functor_h


namespace core {
  FORWARD(Functor);
  FORWARD(Closure);
  FORWARD(BuiltinClosure);
  FORWARD(FunctionClosure);
  FORWARD(InterpretedClosure);
  FORWARD(CompiledClosure);
  FORWARD(Creator);
  FORWARD(InstanceCreator);
};

template <>
struct gctools::GCInfo<core::Functor_O> {
  static bool constexpr CanAllocateWithNoArguments = false;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};
template <>
struct gctools::GCInfo<core::Closure_O> {
  static bool constexpr CanAllocateWithNoArguments = false;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};

template <>
struct gctools::GCInfo<core::InstanceCreator_O> {
  static bool constexpr CanAllocateWithNoArguments = false;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};
template <>
struct gctools::GCInfo<core::FunctionClosure_O> {
  static bool constexpr CanAllocateWithNoArguments = false;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};
template <>
struct gctools::GCInfo<core::BuiltinClosure_O> {
  static bool constexpr CanAllocateWithNoArguments = false;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};
template <>
struct gctools::GCInfo<core::InstanceClosure_O> {
  static bool constexpr CanAllocateWithNoArguments = false;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};
template <>
struct gctools::GCInfo<core::InterpretedClosure_O> {
  static bool constexpr CanAllocateWithNoArguments = false;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};
template <>
struct gctools::GCInfo<core::CompiledClosure_O> {
  static bool constexpr CanAllocateWithNoArguments = false;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};
template <>
struct gctools::GCInfo<core::Creator_O> {
  static bool constexpr CanAllocateWithNoArguments = false;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};

namespace core {
  class Functor_O : public General_O {
    LISP_ABSTRACT_CLASS(core,CorePkg,Functor_O,"Functor",General_O);
public:
  virtual const char *describe() const { return "Functoid - subclass must implement describe()"; };
  inline LCC_RETURN operator()(LCC_ARGS_ELLIPSIS) {
    VaList_S lcc_arglist_s;
    va_start(lcc_arglist_s._Args, LCC_VA_START_ARG);
    LCC_SPILL_REGISTER_ARGUMENTS_TO_VA_LIST(lcc_arglist_s);
    core::T_O *lcc_arglist = lcc_arglist_s.asTaggedPtr();
    return this->invoke_va_list(LCC_PASS_ARGS);
  }

  LCC_VIRTUAL LCC_RETURN LISP_CALLING_CONVENTION() {
    printf("Subclass of Functoid must implement 'activate'\n");
    exit(1);
  };
  virtual size_t templatedSizeof() const { return sizeof(*this); };

public:
  T_sp name;

public:
  Functor_O(T_sp name);
  string nameAsString();
};
};

namespace core {
  struct ClosureWithSlots_O : public Functor_O {
  public:
    typedef T_sp value_type;
  public:
    fnLispCallingConvention _FunctionPointer;
    gctools::GCArray_moveable<value_type,0> _Slots;
  ClosureWithSlots_O(size_t num_slots, T_sp name, fnLispCallingConvention fptr=NULL )
    : Functor_O(name)
      , _Slots(num_slots) {}
  };

  inline CL_DEFUN size_t core__sizeof_header_and_closure_with_slots(size_t numberOfSlots) {
    return gc::sizeof_container_with_header<ClosureWithSlots_O>(numberOfSlots);
  };

#if 0
  inline gctools::tagged_ptr<ClosureWithSlots> initialize_closure_with_slots(void* block, T_sp name, fnLispCallingConvention fptr, size_t num_slots, ... ) {
    gctools::Header_s* header = reinterpret_cast<gctools::Header_s*>(block);
    ClosureWithSlots* closure = reinterpret_cast<ClosureWithSlots*>((char*)block+gctools::global_alignup_sizeof_header);
    new(header) gctools::Header_s(gctools::GCKind<ClosureWithSlots>::Kind);
    new(closure) ClosureWithSlots(name,fptr,num_slots);
    va_list valist;
    va_start(valist, num_slots);
    for ( size_t i=0; i<num_slots; ++i ) {
      closure->_Slots[i] = ClosureWithSlots::value_type(va_arg(valist,T_O*));
    }
    va_end(valist);
    return gctools::tagged_ptr<ClosureWithSlots>((gctools::Tagged)gctools::tag_general<ClosureWithSlots*>(closure));
  }

  inline ClosureWithSlots::value_type closure_with_slots_read_slot(gctools::tagged_ptr<ClosureWithSlots> tagged_closure, size_t index)
  {
    ClosureWithSlots* closure = gctools::untag_general(tagged_closure.theObject);
    return closure->_Slots[index];
  }

  inline void closure_with_slots_write_slot(gctools::tagged_ptr<ClosureWithSlots> tagged_closure, size_t index, ClosureWithSlots::value_type val)
  {
    ClosureWithSlots* closure = gctools::untag_general(tagged_closure.theObject);
    closure->_Slots[index] = val;
  }
#endif
};



namespace core { 
class Closure_O : public Functor_O {
    LISP_CLASS(core,CorePkg,Closure_O,"Closure",Functor_O);
public:
  T_sp closedEnvironment;
public:
  Closure_O(T_sp name, T_sp env) : Functor_O(name), closedEnvironment(env){};
public:
  virtual void setAssociatedFunctions(core::List_sp assocFuncs){};
  virtual const char *describe() const { return "Closure"; };
  LCC_VIRTUAL LCC_RETURN LISP_CALLING_CONVENTION() {
    printf("Subclass of Closure must implement 'activate'\n");
    exit(1);
  };

  virtual void *functionAddress() const { return NULL; };
  virtual T_sp sourcePosInfo() const { return _Nil<T_O>(); };
  virtual bool macroP() const = 0;
  virtual void setKind(Symbol_sp k) = 0;
  virtual Symbol_sp getKind() const = 0;
  virtual bool compiledP() const { return false; };
  virtual bool interpretedP() const { return false; };
  virtual bool builtinP() const { return false; };
  virtual int sourceFileInfoHandle() const;
  virtual size_t filePos() const { return 0; }
  virtual int lineNumber() const { return 0; }
  virtual int column() const { return 0; };
  virtual LambdaListHandler_sp lambdaListHandler() const = 0;
  virtual T_sp lambdaList() const = 0;
  virtual void setf_lambda_list(T_sp lambda_list) = 0;
  virtual T_sp docstring() const;
  virtual List_sp declares() const;
  virtual T_sp cleavir_ast() const;
  virtual void setf_cleavir_ast(T_sp ast);
  virtual T_sp setSourcePosInfo(T_sp sourceFile, size_t filePos, int lineno, int column);
};
};


namespace core {

  class Creator_O : public General_O {
    LISP_CLASS(core,CorePkg,Creator_O,"Creator",General_O);
  public:
  // Some Creators don't actually allocate anything -
  // classes that don't have default allocators
    virtual bool allocates() const { return true; };
  /*! If this is the allocator for a primary CxxAdapter class then return true, */
    virtual int duplicationLevel() const { return 0; };
    virtual size_t templatedSizeof() const = 0;
    virtual Creator_sp duplicateForClassName(core::Symbol_sp className) {
      printf("Subclass must implement Creator::duplicateForClassName\n");
      exit(1);
    };
    virtual void describe() const = 0;
    virtual core::T_sp allocate() = 0;
  };



    template <class _W_>
    class LispObjectCreator_O : public core::Creator_O {
  public:
    typedef core::Creator_O TemplatedBase;
  public:
    DISABLE_NEW();
    size_t templatedSizeof() const { return sizeof(LispObjectCreator_O<_W_>); };
    virtual void describe() const {
      printf("LispObjectCreator for class %s  sizeof_instances-> %zu\n", _rep_(reg::lisp_classSymbol<_W_>()).c_str(), sizeof(_W_));
    }
    virtual core::T_sp allocate() {
      GC_ALLOCATE(_W_, obj);
      return obj;
    }
    virtual void searcher(){};
  };


    class InstanceCreator_O : public Creator_O {
public:
  Symbol_sp _className;
public:
  DISABLE_NEW();
  InstanceCreator_O(Symbol_sp className) : _className(className){};
  void describe() const {
    printf("InstanceAllocatorFunctor for class %s\n", _rep_(this->_className).c_str());
  };
  T_sp allocate();
  virtual size_t templatedSizeof() const { return sizeof(InstanceCreator_O); };
};



};

namespace core {
class FunctionClosure_O : public Closure_O {
  LISP_CLASS(core,CorePkg,FunctionClosure_O,"FunctionClosure",General_O);
 public:
  //  T_sp _SourcePosInfo;
  Symbol_sp kind;
  T_sp _cleavir_ast;
  Fixnum _sourceFileInfoHandle;
  Fixnum _filePos;
  Fixnum _lineno;
  Fixnum _column;

public:
#define SOURCE_INFO core::Fixnum sourceFileInfoHandle, core::Fixnum filePos, core::Fixnum lineno, core::Fixnum column
#define SOURCE_INFO_PASS sourceFileInfoHandle, filePos, lineno, column
  FunctionClosure_O(T_sp name, Symbol_sp k, T_sp env, SOURCE_INFO)
      : Closure_O(name, env), kind(k), _cleavir_ast(_Nil<T_O>()), _sourceFileInfoHandle(sourceFileInfoHandle), _filePos(filePos), _lineno(lineno), _column(column){};
  FunctionClosure_O(T_sp name)
      : Closure_O(name, _Nil<T_O>()), kind(kw::_sym_function), _cleavir_ast(_Nil<T_O>()), _sourceFileInfoHandle(0), _filePos(0), _lineno(0), _column(0){};

  virtual size_t templatedSizeof() const { return sizeof(*this); };

  virtual const char *describe() const { return "FunctionClosure"; };
  LCC_VIRTUAL LCC_RETURN LISP_CALLING_CONVENTION() { SIMPLE_ERROR(BF("Subclass must implement")); };
  void setKind(Symbol_sp k) { this->kind = k; };
  Symbol_sp getKind() const { return this->kind; };
  bool macroP() const;
  T_sp sourcePosInfo() const; // { return this->_SourcePosInfo; };
  virtual T_sp setSourcePosInfo(T_sp sourceFile, size_t filePos, int lineno, int column);
  virtual int sourceFileInfoHandle() const;
  virtual size_t filePos() const;
  virtual int lineNumber() const;
  virtual int column() const;
  virtual T_sp cleavir_ast() const { return this->_cleavir_ast; };
  virtual void setf_cleavir_ast(T_sp ast) { this->_cleavir_ast = ast; };
};

class BuiltinClosure_O : public FunctionClosure_O {
    LISP_CLASS(core,CorePkg,BuiltinClosure_O,"BuiltinClosure",FunctionClosure_O);
public:
  LambdaListHandler_sp _lambdaListHandler;
public:
  BuiltinClosure_O(T_sp name, Symbol_sp k, SOURCE_INFO)
      : FunctionClosure_O(name, k, _Nil<T_O>(), SOURCE_INFO_PASS){};
  BuiltinClosure_O(T_sp name)
      : FunctionClosure_O(name) {}
  void finishSetup(LambdaListHandler_sp llh, Symbol_sp k) {
    this->_lambdaListHandler = llh;
    this->kind = k;
  }
  virtual T_sp lambdaList() const;
  virtual void setf_lambda_list(T_sp lambda_list);
  virtual size_t templatedSizeof() const { return sizeof(*this); };
  virtual const char *describe() const { return "BuiltinClosure"; };
  LCC_VIRTUAL LCC_RETURN LISP_CALLING_CONVENTION();
  bool builtinP() const { return true; };
  LambdaListHandler_sp lambdaListHandler() const { return this->_lambdaListHandler; };
};

/*! Shouldn't this derive from a Functoid - it doesn't need a closedEnvironment */
class InstanceClosure_O : public FunctionClosure_O {
    LISP_CLASS(core,CorePkg,InstanceClosure_O,"InstanceClosure",FunctionClosure_O);
public:
  GenericFunctionPtr entryPoint;
  Instance_sp instance;
  T_sp lambda_list;
public:
  InstanceClosure_O(T_sp name, GenericFunctionPtr ep, Instance_sp inst)
    : FunctionClosure_O(name), entryPoint(ep), instance(inst), lambda_list(_Nil<T_O>()){};
  virtual size_t templatedSizeof() const { return sizeof(*this); };
  virtual const char *describe() const { return "InstanceClosure"; };
  LCC_VIRTUAL LCC_RETURN LISP_CALLING_CONVENTION();
  LambdaListHandler_sp lambdaListHandler() const { return _Nil<LambdaListHandler_O>(); };
  T_sp lambdaList() const { return this->lambda_list; };
  void setf_lambda_list(T_sp ll) { this->lambda_list = ll; };
};
}


namespace core {
class InterpretedClosure_O : public FunctionClosure_O {
    LISP_CLASS(core,CorePkg,InterpretedClosure_O,"InterpretedClosure",FunctionClosure_O);
public:
  LambdaListHandler_sp _lambdaListHandler;
  List_sp _declares;
  T_sp _docstring;
  List_sp _code;

public:
  DISABLE_NEW();
  InterpretedClosure_O(T_sp fn, Symbol_sp k, LambdaListHandler_sp llh, List_sp dec, T_sp doc, T_sp e, List_sp c, SOURCE_INFO);
  virtual size_t templatedSizeof() const { return sizeof(*this); };
  virtual const char *describe() const { return "InterpretedClosure"; };
  LCC_VIRTUAL LCC_RETURN LISP_CALLING_CONVENTION();
  bool interpretedP() const { return true; };
  T_sp docstring() const { return this->_docstring; };
  List_sp declares() const { return this->_declares; };
  List_sp code() const { return this->_code; };
  LambdaListHandler_sp lambdaListHandler() const { return this->_lambdaListHandler; };
  T_sp lambdaList() const;
  void setf_lambda_list(T_sp lambda_list);
};

};


namespace core {
class CompiledClosure_O : public core::FunctionClosure_O {
//  friend void dump_funcs(core::CompiledFunction_sp compiledFunction);
  LISP_CLASS(core,CorePkg,CompiledClosure_O,"CompiledClosure",core::FunctionClosure_O);
public:
  core::T_sp llvmFunction;
  core::CompiledClosure_fptr_type fptr;
  core::T_sp associatedFunctions;
  core::T_sp _lambdaList;
  //! Slots must be the last field
  typedef core::T_sp value_type;
  gctools::GCArray_moveable<value_type,0> _Slots;
 public:
  virtual const char *describe() const { return "CompiledClosure"; };
  virtual size_t templatedSizeof() const { return sizeof(*this); };
  virtual void *functionAddress() const { return (void *)this->fptr; }

public:
  CompiledClosure_O(core::T_sp functionName, core::Symbol_sp type, core::CompiledClosure_fptr_type ptr, core::T_sp llvmFunc, core::T_sp renv, core::T_sp assocFuncs,
                  core::T_sp ll, SOURCE_INFO)
      : FunctionClosure_O(functionName, type, renv, SOURCE_INFO_PASS), fptr(ptr), associatedFunctions(assocFuncs), _lambdaList(ll){};
  void setAssociatedFunctions(core::List_sp assocFuncs) { this->associatedFunctions = assocFuncs; };
  bool compiledP() const { return true; };
  core::T_sp lambdaList() const;
  void setf_lambda_list(core::T_sp lambda_list);
  core::LambdaListHandler_sp lambdaListHandler() const { return _Nil<core::LambdaListHandler_O>(); };
  DISABLE_NEW();
  inline LCC_RETURN LISP_CALLING_CONVENTION() {
    core::InvocationHistoryFrame _frame(Closure_sp(this), lcc_arglist, this->closedEnvironment);
    core::T_O *closedEnv = LCC_FROM_ACTIVATION_FRAME_SMART_PTR(this->closedEnvironment);
    return (*(this->fptr))(LCC_PASS_ARGS_ENV(closedEnv));
  };
};
};



#endif

