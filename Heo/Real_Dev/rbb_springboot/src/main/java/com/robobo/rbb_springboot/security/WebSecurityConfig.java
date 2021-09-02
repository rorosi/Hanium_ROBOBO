package com.robobo.rbb_springboot.security;


import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.method.configuration.EnableGlobalMethodSecurity;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.builders.WebSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;

@EnableWebSecurity
@Configuration
@EnableGlobalMethodSecurity(securedEnabled = true)
public class WebSecurityConfig extends WebSecurityConfigurerAdapter {

    // 패스워드 암호화 하기위해 Bean 등록 (DI 사용)
    @Bean
    public BCryptPasswordEncoder encodePassword() {
        return new BCryptPasswordEncoder();
    }



    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http.csrf().disable();
        http.headers().frameOptions().disable();

        http
                .authorizeRequests()
                // static 자원들을 login 없이 허용
                    .antMatchers("/","/css/**","/js/**", "/scss/**",
                            "/vendor/**","/img/**","/h2-console/**","/user/register").permitAll()
                        // 그 외 모든 요청은 인증과정 필요

                        .anyRequest().authenticated()
                        .and()
                    .formLogin()
                        .loginPage("/user/login") // 인증 안돼있으면 로그인페이지로 이동

                        //시큐리티가 로그인프로세스 낚아챔 컨트롤러에 안만들어도된다.
                        .loginProcessingUrl("/user/loginProcess")

                        //성공 시 "/" 이동
                        .defaultSuccessUrl("/")

                        .permitAll() // 모든 사용자에게 허용
                        .and()
                    .logout()
                        .permitAll()
                        .and()
                    .exceptionHandling()
                        .accessDeniedPage("/user/forbidden"); // 접근권한 없을 시 403 페이지 이동
    }
}
