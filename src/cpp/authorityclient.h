#ifndef _AUTHORITYCLIENT_H_
#define _AUTHORITYCLIENT_H_

#include "Authority.h"

namespace roomsec {
  class AuthorityClient : virtual public AuthorityIf {
  public:
    AuthorityClient();
    
    //    AuthorityClient(
    setIp(const string);
    string getIP(void);
    
    void checkRequirements(std::vector<CredentialSpec> & _return, const std::string& resource);
    AuthorizationReply::type authorize(const AuthorizationRequest& request);
  };
}
#endif /* _AUTHORITYCLIENT_H_ */
