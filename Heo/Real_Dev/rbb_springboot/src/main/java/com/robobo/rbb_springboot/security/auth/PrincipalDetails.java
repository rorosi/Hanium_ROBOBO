package com.robobo.rbb_springboot.security.auth;


// 시큐리티가 /user/login post요청을 낚아채서 로그인 진행시킴
// 로그인 진행 완료되면 session을 만들어줌 (Security ContextHolder)
// 오브젝트 타입 => Authentication 타입 객체
// Authentication 안에 User 정보가 있어야 됨
// User 오브젝트 타입 = UserDetails 타입 객체

// Security Session => Authentication 정해져있음 => UserDetails 정해져있음

import com.robobo.rbb_springboot.model.User;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;

import java.util.ArrayList;
import java.util.Collection;

public class PrincipalDetails implements UserDetails {

    private final User user; // 컴포지션

    public PrincipalDetails(User user) {
        this.user = user;
    }

    // 해당 user 의 권한을 리턴하는 곳
    @Override
    public Collection<? extends GrantedAuthority> getAuthorities() {
        Collection<GrantedAuthority> collection = new ArrayList<>();
        collection.add(new GrantedAuthority() {
            @Override
            public String getAuthority() {
                return user.getRole().toString();
            }
        });

        return collection;
    }

    @Override
    public String getPassword() {
        return user.getPassword();
    }

    @Override
    public String getUsername() {
        return user.getUsername();
    }

    // 계정 만료됐냐
    @Override
    public boolean isAccountNonExpired() {
        return true;
    }
    // 계정 잠겼냐
    @Override
    public boolean isAccountNonLocked() {
        return true;
    }

    // 비번 기간 지났나
    @Override
    public boolean isCredentialsNonExpired() {
        return true;
    }

    // 활성화
    @Override
    public boolean isEnabled() {
        // 참고 ) 우리 사이트에서 1년동안 회원이 로그인을 안해서 휴면 계정으로 전환 하려면
        // 현재시간 - 로그인시간 => 1년 초과하면 return false;
        return true;
    }
}
