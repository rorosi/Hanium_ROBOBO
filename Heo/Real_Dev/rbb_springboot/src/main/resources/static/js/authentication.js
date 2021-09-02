function registerUser() {
    $.ajax({
        type: 'POST',
        url: '/user/register',
        contentType: "application/json",
        data: JSON.stringify({
            "username": $('#username').val(),
            "email": $('#email').val(),
            "password": $('#password').val(),
            "tel": $('#tel').val()
        }),
        success: function (response) {
            alert(response + " 님 사용자 등록이 완료됐습니다.");
            window.location.href='/user/login';
        },
        error: function (request, error) {
            alert("사용자 등록 오류입니다. 잠시 후 시도해주세요");
            console.log("code:"+request.status+"\n"+"message:"+request.responseText+"\n"+"error:"+error);
        }
    })
}

