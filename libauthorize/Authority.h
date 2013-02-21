/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Authority_H
#define Authority_H

#include <thrift/TDispatchProcessor.h>
#include "authorize_types.h"

namespace authorize {

class AuthorityIf {
 public:
  virtual ~AuthorityIf() {}
  virtual void checkRequirements(std::vector<CredentialSpec> & _return, const std::string& resource) = 0;
  virtual AuthorizationReply::type request(const AuthorizationRequest& request) = 0;
};

class AuthorityIfFactory {
 public:
  typedef AuthorityIf Handler;

  virtual ~AuthorityIfFactory() {}

  virtual AuthorityIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(AuthorityIf* /* handler */) = 0;
};

class AuthorityIfSingletonFactory : virtual public AuthorityIfFactory {
 public:
  AuthorityIfSingletonFactory(const boost::shared_ptr<AuthorityIf>& iface) : iface_(iface) {}
  virtual ~AuthorityIfSingletonFactory() {}

  virtual AuthorityIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(AuthorityIf* /* handler */) {}

 protected:
  boost::shared_ptr<AuthorityIf> iface_;
};

class AuthorityNull : virtual public AuthorityIf {
 public:
  virtual ~AuthorityNull() {}
  void checkRequirements(std::vector<CredentialSpec> & /* _return */, const std::string& /* resource */) {
    return;
  }
  AuthorizationReply::type request(const AuthorizationRequest& /* request */) {
    AuthorizationReply::type _return = (AuthorizationReply::type)0;
    return _return;
  }
};

typedef struct _Authority_checkRequirements_args__isset {
  _Authority_checkRequirements_args__isset() : resource(false) {}
  bool resource;
} _Authority_checkRequirements_args__isset;

class Authority_checkRequirements_args {
 public:

  Authority_checkRequirements_args() : resource() {
  }

  virtual ~Authority_checkRequirements_args() throw() {}

  std::string resource;

  _Authority_checkRequirements_args__isset __isset;

  void __set_resource(const std::string& val) {
    resource = val;
  }

  bool operator == (const Authority_checkRequirements_args & rhs) const
  {
    if (!(resource == rhs.resource))
      return false;
    return true;
  }
  bool operator != (const Authority_checkRequirements_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Authority_checkRequirements_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Authority_checkRequirements_pargs {
 public:


  virtual ~Authority_checkRequirements_pargs() throw() {}

  const std::string* resource;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Authority_checkRequirements_result__isset {
  _Authority_checkRequirements_result__isset() : success(false) {}
  bool success;
} _Authority_checkRequirements_result__isset;

class Authority_checkRequirements_result {
 public:

  Authority_checkRequirements_result() {
  }

  virtual ~Authority_checkRequirements_result() throw() {}

  std::vector<CredentialSpec>  success;

  _Authority_checkRequirements_result__isset __isset;

  void __set_success(const std::vector<CredentialSpec> & val) {
    success = val;
  }

  bool operator == (const Authority_checkRequirements_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Authority_checkRequirements_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Authority_checkRequirements_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Authority_checkRequirements_presult__isset {
  _Authority_checkRequirements_presult__isset() : success(false) {}
  bool success;
} _Authority_checkRequirements_presult__isset;

class Authority_checkRequirements_presult {
 public:


  virtual ~Authority_checkRequirements_presult() throw() {}

  std::vector<CredentialSpec> * success;

  _Authority_checkRequirements_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Authority_request_args__isset {
  _Authority_request_args__isset() : request(false) {}
  bool request;
} _Authority_request_args__isset;

class Authority_request_args {
 public:

  Authority_request_args() {
  }

  virtual ~Authority_request_args() throw() {}

  AuthorizationRequest request;

  _Authority_request_args__isset __isset;

  void __set_request(const AuthorizationRequest& val) {
    request = val;
  }

  bool operator == (const Authority_request_args & rhs) const
  {
    if (!(request == rhs.request))
      return false;
    return true;
  }
  bool operator != (const Authority_request_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Authority_request_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Authority_request_pargs {
 public:


  virtual ~Authority_request_pargs() throw() {}

  const AuthorizationRequest* request;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Authority_request_result__isset {
  _Authority_request_result__isset() : success(false) {}
  bool success;
} _Authority_request_result__isset;

class Authority_request_result {
 public:

  Authority_request_result() : success((AuthorizationReply::type)0) {
  }

  virtual ~Authority_request_result() throw() {}

  AuthorizationReply::type success;

  _Authority_request_result__isset __isset;

  void __set_success(const AuthorizationReply::type val) {
    success = val;
  }

  bool operator == (const Authority_request_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Authority_request_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Authority_request_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Authority_request_presult__isset {
  _Authority_request_presult__isset() : success(false) {}
  bool success;
} _Authority_request_presult__isset;

class Authority_request_presult {
 public:


  virtual ~Authority_request_presult() throw() {}

  AuthorizationReply::type* success;

  _Authority_request_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class AuthorityClient : virtual public AuthorityIf {
 public:
  AuthorityClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  AuthorityClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void checkRequirements(std::vector<CredentialSpec> & _return, const std::string& resource);
  void send_checkRequirements(const std::string& resource);
  void recv_checkRequirements(std::vector<CredentialSpec> & _return);
  AuthorizationReply::type request(const AuthorizationRequest& request);
  void send_request(const AuthorizationRequest& request);
  AuthorizationReply::type recv_request();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class AuthorityProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<AuthorityIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (AuthorityProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_checkRequirements(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_request(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  AuthorityProcessor(boost::shared_ptr<AuthorityIf> iface) :
    iface_(iface) {
    processMap_["checkRequirements"] = &AuthorityProcessor::process_checkRequirements;
    processMap_["request"] = &AuthorityProcessor::process_request;
  }

  virtual ~AuthorityProcessor() {}
};

class AuthorityProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  AuthorityProcessorFactory(const ::boost::shared_ptr< AuthorityIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< AuthorityIfFactory > handlerFactory_;
};

class AuthorityMultiface : virtual public AuthorityIf {
 public:
  AuthorityMultiface(std::vector<boost::shared_ptr<AuthorityIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~AuthorityMultiface() {}
 protected:
  std::vector<boost::shared_ptr<AuthorityIf> > ifaces_;
  AuthorityMultiface() {}
  void add(boost::shared_ptr<AuthorityIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void checkRequirements(std::vector<CredentialSpec> & _return, const std::string& resource) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->checkRequirements(_return, resource);
    }
    ifaces_[i]->checkRequirements(_return, resource);
    return;
  }

  AuthorizationReply::type request(const AuthorizationRequest& request) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->request(request);
    }
    return ifaces_[i]->request(request);
  }

};

} // namespace

#endif
