package com.robobo.rbb_springboot.security.oauth;

import com.robobo.rbb_springboot.model.User;
import com.robobo.rbb_springboot.model.UserRole;
import com.robobo.rbb_springboot.repository.UserRepository;
import com.robobo.rbb_springboot.security.auth.PrincipalDetails;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Lazy;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.oauth2.client.userinfo.DefaultOAuth2UserService;
import org.springframework.security.oauth2.client.userinfo.OAuth2UserRequest;
import org.springframework.security.oauth2.core.OAuth2AuthenticationException;
import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.stereotype.Service;


@Service
public class PrincipalOauth2UserService extends DefaultOAuth2UserService {


    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;


    public PrincipalOauth2UserService(UserRepository userRepository, PasswordEncoder passwordEncoder) {
        this.userRepository = userRepository;
        this.passwordEncoder = passwordEncoder;
    }


    // google 에서 받은 userRequest 데이터에 대한 후처리 함수
    @Override
    public OAuth2User loadUser(OAuth2UserRequest userRequest) throws OAuth2AuthenticationException {

        System.out.println("getClientRegistration:" + userRequest.getClientRegistration());
        System.out.println("getAccessToken:" + userRequest.getAccessToken().getTokenValue());

        OAuth2User oAuth2User = super.loadUser(userRequest);
        // 구글로그인 -> 로그인완료 -> code를 리턴(OAuth-client 라이브러리) -> Access Token 요청
        // userRequest 정보 -> loadUser 함수 호출 -> 구글로부터 회원 프로필 받아준다.
        System.out.println("getAttribute:" + oAuth2User.getAttributes());

        //회원가입을 강제로(자동으로) 진행
        String provider = userRequest.getClientRegistration().getClientId(); // google
        String providerId = oAuth2User.getAttribute("sub"); // 1023231~~~
        String username = oAuth2User.getAttribute("name");
        String password = passwordEncoder.encode("비밀번호");

        String email = oAuth2User.getAttribute("email");
        UserRole role = UserRole.ROLE_USER;

        User user = userRepository.findByEmail(email).orElse(null);

        if (user == null) {
            user = User.builder()
                    .username(username)
                    .password(password)
                    .email(email)
                    .tel(provider) // 소셜 로그인 시에는 번호 없이 provider 를 넣어주기로 함
                    .role(role)
                    .provider(provider)
                    .providerId(providerId)
                    .build();
            userRepository.save(user);
        }

        // PrincipalDetails -> OAuth 생성자 -> 생성해서 Authentication 객체에 전달
        return new PrincipalDetails(user, oAuth2User.getAttributes());
    }
}
