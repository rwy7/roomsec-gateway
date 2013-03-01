#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "gen-cpp/authorize_types.h"
#include "mockauthorityadapter.h"

/*
 * Authority adapter test.  This test examines if the authority
 * adapter interface is correctly calling and working with it's
 * subclass implementations.
 */


namespace roomsec {
  using namespace ::testing;

  TEST(AuthorityAdapterTest, CallImplementation) {
    MockAuthorityAdapter authorityAdapter;

    std::string resource = "door/open";
    std::vector<iface::CredentialSpec> requirements(5);

    iface::AuthorizationRequest request;
    request.resource = resource;
    request.credentials = std::vector<iface::Credential>(5);

    EXPECT_CALL(authorityAdapter, checkRequirementsImpl(_,_))
      .Times(AtLeast(1))
      .WillRepeatedly(Return());

    EXPECT_CALL(authorityAdapter, authorizeImpl(_))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(iface::AuthorizationReply::grant));

    EXPECT_EQ(authorityAdapter.authorize(request),
	      iface::AuthorizationReply::grant);

    authorityAdapter.checkRequirements(requirements, resource);
  }
}
