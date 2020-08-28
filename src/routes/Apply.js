import React from "react"
import "./Apply.css"

function Apply() {

    return (
        <>
        {/*Masthead*/}
        <header class="masthead" id="page-top"> 
        <div class="container h-100">
            <div class="row h-100 align-items-center justify-content-center text-center">
                <div class="col-lg-10 align-self-end">
                    <h1 class="title"> 티키타카 <br /> </h1> {/* text-uppercase라는 class를 주고 그 클래스는 다 속성을 줌으로써 클래스 명만으로 파악하도록 했다  */}
                    <h2 class="sub_title">온라인 미팅 프로젝트 </h2>
                    <hr class="divider my-4" />
                </div>
                <div class="col-lg-8 align-self-baseline">
                    <p class="text-white-75 font-weight-light mb-5">Start Bootstrap can help you build better websites using the Bootstrap framework! Just download a theme and start customizing, no strings attached!
                    <br/><br/>
                    모집기간 : 9/00 ~ 9/00
                    </p>
                    <a class="btn btn-primary btn-xl js-scroll-trigger" href="#about"> 신청하기 </a>
                </div>
            </div>
        </div>
    </header>
            {/* <div className="Main" id="main">
                <p id="title"> 티키타카 </p>
                <span id="title_explain">
                
                <p>문의 사항이 있는 경우 언제든지 편하게 <a href="https://open.kakao.com/o/sfQGOIrc">오픈 카톡<img id="icon_kakaotalk" src="data:image/svg+xml;base64,PHN2ZyBlbmFibGUtYmFja2dyb3VuZD0ibmV3IDAgMCAyNCAyNCIgaGVpZ2h0PSI1MTIiIHZpZXdCb3g9IjAgMCAyNCAyNCIgd2lkdGg9IjUxMiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj48cGF0aCBkPSJtMTIgMWMtNi42MjcgMC0xMiA0LjIwOC0xMiA5LjM5OSAwIDMuMzU2IDIuMjQ2IDYuMzAxIDUuNjI1IDcuOTYzLTEuNjc4IDUuNzQ5LTIuNjY0IDYuMTIzIDQuMjQ0IDEuMjg3LjY5Mi4wOTcgMS40MDQuMTQ4IDIuMTMxLjE0OCA2LjYyNyAwIDEyLTQuMjA4IDEyLTkuMzk5IDAtNS4xOS01LjM3My05LjM5OC0xMi05LjM5OHoiIGZpbGw9IiMzZTI3MjMiLz48ZyBmaWxsPSIjZmZlYjNiIj48cGF0aCBkPSJtMTAuMzg0IDguMjdjLS4zMTctLjg5My0xLjUyOS0uODk0LTEuODQ1LS4wMDEtLjk4NCAzLjA1Mi0yLjMwMiA0LjkzNS0xLjQ5MiA1LjMwNiAxLjA3OC40ODkgMS4xMDEtLjYxMSAxLjM1OS0xLjFoMi4xMTFjLjI1Ny40ODcuMjgyIDEuNTg4IDEuMzU5IDEuMS44MTMtLjM3MS0uNDg5LTIuMTk1LTEuNDkyLTUuMzA1em0tMS42MTQgMi45ODcuNjkyLTEuOTUxLjY5MSAxLjk1MXoiLz48cGF0aCBkPSJtNS4zNjUgMTMuNjhjLTEuMTk4IDAtLjQ5LTEuNjU3LS42OTItNC43NDItLjQyOS0uMDc0LTEuNzYuMjk3LTEuNzYtLjY3MyAwLS4zNzEuMzA1LS42NzMuNjc5LS42NzMgMi41MTguMTggNC4yMjQtLjQ3IDQuMjI0LjY3MyAwIC45ODctMS4yNzUuNTktMS43Ni42NzMtLjIgMy4wNzUuNTA1IDQuNzQyLS42OTEgNC43NDJ6Ii8+PHBhdGggZD0ibTEzLjE1NCAxMy41NzljLTEuMTU5IDAtLjQ1NC0xLjU2NS0uNjYzLTUuMzAxIDAtLjkxIDEuNDEzLS45MDkgMS40MTMgMHY0LjA0Yy42NjkuMDg5IDIuMTM1LS4zMyAyLjEzNS42My0uMDAxIDEuMDA3LTEuNTc2LjUwMy0yLjg4NS42MzF6Ii8+PHBhdGggZD0ibTE5LjU1NiAxMy4zOC0xLjYyNC0yLjEzNy0uMjQuMjM5djEuNWMwIC4zOC0uMzEuNjg4LS42OTMuNjg4LTEuMjAzIDAtLjQ4Mi0xLjczMi0uNjkyLTUuMzkyIDAtLjM3OS4zMS0uNjg4LjY5Mi0uNjg4IDEuMDQ1IDAgLjU5NCAxLjQ3OC42OTIgMi4xNjYgMS45Ni0xLjg3MyAxLjkxMy0yLjA3MiAyLjMxNi0yLjA3Mi41NTYgMCAuODk3LjY5MS41MjcgMS4wNThsLTEuNTc4IDEuNTY3IDEuNzA0IDIuMjQzYy41NTYuNzI1LS41NTUgMS41NTYtMS4xMDQuODI4eiIvPjwvZz48L3N2Zz4=" alt="openkakaotalk" /></a>으로 말씀해주시면, 담당자가 최대한 빠르게 해결해 드리겠습니다🙋‍♂️</p>
                <p>감사합니다! </p>

                <p id="durate"> 모집기간 8/00 ~ 8/00 </p>
                </span>
                <a id="apply" href="https://bit.ly/2YwmKNe"> 신청하기 </a>
            </div>

            <div className="controller">
                    <li> <a href="#main"> 신청 </a> </li>
                    <li> <a href="#explain"> 설명 </a> </li>
                    <li> <a href="#About"> QnA </a> </li>
            </div> */}
        </>
    )
}


export default Apply;