package com.robobo.rbb_springboot;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.servlet.ServletComponentScan;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;

@SpringBootApplication
@EnableJpaAuditing
@ServletComponentScan
public class RbbSpringbootApplication {

    public static void main(String[] args) {
        SpringApplication.run(RbbSpringbootApplication.class, args);
    }

}
