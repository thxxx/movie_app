import React from "react"
import "./QnA.css"

function QnA() {

    return (
        <div id="About">
            <span className="qna">
                <p id="qna_title"> <b> Questions & Answers </b> </p>
                <b> Q. 상대방에 대한 정보는 언제 알 수 있나요? </b> <br />
        A. 상대는 저희측에서 매칭해드리고 미팅을 시작하면 알 수 있습니다. <br /><br />
        <b> Q. 일반 미팅과 다른 점이 뭔가요? </b> <br />
        A. Zoom을 통해 온라인으로 진행되어 훨씬 편하고 그에 맞게 게임을 자체제작하여서 원활하게 즐기실 수 있습니다~~ <br /><br />
        <b> Q. 보안과 안전에 관해 믿을 수 있나요? </b> <br />
        A. 안전을 위해 참여자 본인 신분 확인과정을 거치며 온라인이라는 특성상 발생할 수 있는 <br />
        문제들에 관해 주의사항을 확실히 숙지시키고 그에 따른 조치방법, 대응메뉴얼 등을 <br />
        만들어두었으므로 걱정하지않으셔도 됩니다. <br /><br />
        <b> Q. 보증금을 받나요? </b> <br />
        A. 네! 보증금은 다른 인원들이 준비하고 있는 상황에서 갑작스런 참여의사철회, 노쇼 등과 같은 경우에 <br />
        대비하기위해 받고 있습니다. 이런경우가 아니라면 100% 돌려드립니다. <br />
            </span>

            <div className="more_information">
                <p id="moreinf_title"> <b>문의사항 </b> </p>
                <p> 더 자세한 정보를 알고싶다면 아래사이트를 확인하거나 오픈카카오톡으로 문의주세요!  </p>

                <ul>
                    <li><a href="https://www.facebook.com/tiki_taka_meet-113715983784023/">
                        <img src="data:image/svg+xml;base64,PHN2ZyBlbmFibGUtYmFja2dyb3VuZD0ibmV3IDAgMCAyNCAyNCIgaGVpZ2h0PSI1MTIiIHZpZXdCb3g9IjAgMCAyNCAyNCIgd2lkdGg9IjUxMiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj48cGF0aCBkPSJtMjEgMGgtMThjLTEuNjU1IDAtMyAxLjM0NS0zIDN2MThjMCAxLjY1NCAxLjM0NSAzIDMgM2gxOGMxLjY1NCAwIDMtMS4zNDYgMy0zdi0xOGMwLTEuNjU1LTEuMzQ2LTMtMy0zeiIgZmlsbD0iIzNiNTk5OSIvPjxwYXRoIGQ9Im0xNi41IDEydi0zYzAtLjgyOC42NzItLjc1IDEuNS0uNzVoMS41di0zLjc1aC0zYy0yLjQ4NiAwLTQuNSAyLjAxNC00LjUgNC41djNoLTN2My43NWgzdjguMjVoNC41di04LjI1aDIuMjVsMS41LTMuNzV6IiBmaWxsPSIjZmZmIi8+PC9zdmc+" alt="페이스북" />
                        <p> 페이스북 </p>
                        </a>
                        </li>
                    
                    <li><a href="https://www.instagram.com/official_tiki_taka/"><img src="data:image/svg+xml;base64,PHN2ZyBlbmFibGUtYmFja2dyb3VuZD0ibmV3IDAgMCAyNCAyNCIgaGVpZ2h0PSI1MTIiIHZpZXdCb3g9IjAgMCAyNCAyNCIgd2lkdGg9IjUxMiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxuczp4bGluaz0iaHR0cDovL3d3dy53My5vcmcvMTk5OS94bGluayI+PGxpbmVhckdyYWRpZW50IGlkPSJTVkdJRF8xXyIgZ3JhZGllbnRUcmFuc2Zvcm09Im1hdHJpeCgwIC0xLjk4MiAtMS44NDQgMCAtMTMyLjUyMiAtNTEuMDc3KSIgZ3JhZGllbnRVbml0cz0idXNlclNwYWNlT25Vc2UiIHgxPSItMzcuMTA2IiB4Mj0iLTI2LjU1NSIgeTE9Ii03Mi43MDUiIHkyPSItODQuMDQ3Ij48c3RvcCBvZmZzZXQ9IjAiIHN0b3AtY29sb3I9IiNmZDUiLz48c3RvcCBvZmZzZXQ9Ii41IiBzdG9wLWNvbG9yPSIjZmY1NDNlIi8+PHN0b3Agb2Zmc2V0PSIxIiBzdG9wLWNvbG9yPSIjYzgzN2FiIi8+PC9saW5lYXJHcmFkaWVudD48cGF0aCBkPSJtMS41IDEuNjMzYy0xLjg4NiAxLjk1OS0xLjUgNC4wNC0xLjUgMTAuMzYyIDAgNS4yNS0uOTE2IDEwLjUxMyAzLjg3OCAxMS43NTIgMS40OTcuMzg1IDE0Ljc2MS4zODUgMTYuMjU2LS4wMDIgMS45OTYtLjUxNSAzLjYyLTIuMTM0IDMuODQyLTQuOTU3LjAzMS0uMzk0LjAzMS0xMy4xODUtLjAwMS0xMy41ODctLjIzNi0zLjAwNy0yLjA4Ny00Ljc0LTQuNTI2LTUuMDkxLS41NTktLjA4MS0uNjcxLS4xMDUtMy41MzktLjExLTEwLjE3My4wMDUtMTIuNDAzLS40NDgtMTQuNDEgMS42MzN6IiBmaWxsPSJ1cmwoI1NWR0lEXzFfKSIvPjxwYXRoIGQ9Im0xMS45OTggMy4xMzljLTMuNjMxIDAtNy4wNzktLjMyMy04LjM5NiAzLjA1Ny0uNTQ0IDEuMzk2LS40NjUgMy4yMDktLjQ2NSA1LjgwNSAwIDIuMjc4LS4wNzMgNC40MTkuNDY1IDUuODA0IDEuMzE0IDMuMzgyIDQuNzkgMy4wNTggOC4zOTQgMy4wNTggMy40NzcgMCA3LjA2Mi4zNjIgOC4zOTUtMy4wNTguNTQ1LTEuNDEuNDY1LTMuMTk2LjQ2NS01LjgwNCAwLTMuNDYyLjE5MS01LjY5Ny0xLjQ4OC03LjM3NS0xLjctMS43LTMuOTk5LTEuNDg3LTcuMzc0LTEuNDg3em0tLjc5NCAxLjU5N2M3LjU3NC0uMDEyIDguNTM4LS44NTQgOC4wMDYgMTAuODQzLS4xODkgNC4xMzctMy4zMzkgMy42ODMtNy4yMTEgMy42ODMtNy4wNiAwLTcuMjYzLS4yMDItNy4yNjMtNy4yNjUgMC03LjE0NS41Ni03LjI1NyA2LjQ2OC03LjI2M3ptNS41MjQgMS40NzFjLS41ODcgMC0xLjA2My40NzYtMS4wNjMgMS4wNjNzLjQ3NiAxLjA2MyAxLjA2MyAxLjA2MyAxLjA2My0uNDc2IDEuMDYzLTEuMDYzLS40NzYtMS4wNjMtMS4wNjMtMS4wNjN6bS00LjczIDEuMjQzYy0yLjUxMyAwLTQuNTUgMi4wMzgtNC41NSA0LjU1MXMyLjAzNyA0LjU1IDQuNTUgNC41NSA0LjU0OS0yLjAzNyA0LjU0OS00LjU1LTIuMDM2LTQuNTUxLTQuNTQ5LTQuNTUxem0wIDEuNTk3YzMuOTA1IDAgMy45MSA1LjkwOCAwIDUuOTA4LTMuOTA0IDAtMy45MS01LjkwOCAwLTUuOTA4eiIgZmlsbD0iI2ZmZiIvPjwvc3ZnPg==" alt="인스타" />
                    <p> 인스타그램 </p>
                    </a>
                    </li>
                    
                    <li><a href="https://open.kakao.com/o/sfQGOIrc"><img src="data:image/svg+xml;base64,PHN2ZyBlbmFibGUtYmFja2dyb3VuZD0ibmV3IDAgMCAyNCAyNCIgaGVpZ2h0PSI1MTIiIHZpZXdCb3g9IjAgMCAyNCAyNCIgd2lkdGg9IjUxMiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj48cGF0aCBkPSJtMTIgMWMtNi42MjcgMC0xMiA0LjIwOC0xMiA5LjM5OSAwIDMuMzU2IDIuMjQ2IDYuMzAxIDUuNjI1IDcuOTYzLTEuNjc4IDUuNzQ5LTIuNjY0IDYuMTIzIDQuMjQ0IDEuMjg3LjY5Mi4wOTcgMS40MDQuMTQ4IDIuMTMxLjE0OCA2LjYyNyAwIDEyLTQuMjA4IDEyLTkuMzk5IDAtNS4xOS01LjM3My05LjM5OC0xMi05LjM5OHoiIGZpbGw9IiMzZTI3MjMiLz48ZyBmaWxsPSIjZmZlYjNiIj48cGF0aCBkPSJtMTAuMzg0IDguMjdjLS4zMTctLjg5My0xLjUyOS0uODk0LTEuODQ1LS4wMDEtLjk4NCAzLjA1Mi0yLjMwMiA0LjkzNS0xLjQ5MiA1LjMwNiAxLjA3OC40ODkgMS4xMDEtLjYxMSAxLjM1OS0xLjFoMi4xMTFjLjI1Ny40ODcuMjgyIDEuNTg4IDEuMzU5IDEuMS44MTMtLjM3MS0uNDg5LTIuMTk1LTEuNDkyLTUuMzA1em0tMS42MTQgMi45ODcuNjkyLTEuOTUxLjY5MSAxLjk1MXoiLz48cGF0aCBkPSJtNS4zNjUgMTMuNjhjLTEuMTk4IDAtLjQ5LTEuNjU3LS42OTItNC43NDItLjQyOS0uMDc0LTEuNzYuMjk3LTEuNzYtLjY3MyAwLS4zNzEuMzA1LS42NzMuNjc5LS42NzMgMi41MTguMTggNC4yMjQtLjQ3IDQuMjI0LjY3MyAwIC45ODctMS4yNzUuNTktMS43Ni42NzMtLjIgMy4wNzUuNTA1IDQuNzQyLS42OTEgNC43NDJ6Ii8+PHBhdGggZD0ibTEzLjE1NCAxMy41NzljLTEuMTU5IDAtLjQ1NC0xLjU2NS0uNjYzLTUuMzAxIDAtLjkxIDEuNDEzLS45MDkgMS40MTMgMHY0LjA0Yy42NjkuMDg5IDIuMTM1LS4zMyAyLjEzNS42My0uMDAxIDEuMDA3LTEuNTc2LjUwMy0yLjg4NS42MzF6Ii8+PHBhdGggZD0ibTE5LjU1NiAxMy4zOC0xLjYyNC0yLjEzNy0uMjQuMjM5djEuNWMwIC4zOC0uMzEuNjg4LS42OTMuNjg4LTEuMjAzIDAtLjQ4Mi0xLjczMi0uNjkyLTUuMzkyIDAtLjM3OS4zMS0uNjg4LjY5Mi0uNjg4IDEuMDQ1IDAgLjU5NCAxLjQ3OC42OTIgMi4xNjYgMS45Ni0xLjg3MyAxLjkxMy0yLjA3MiAyLjMxNi0yLjA3Mi41NTYgMCAuODk3LjY5MS41MjcgMS4wNThsLTEuNTc4IDEuNTY3IDEuNzA0IDIuMjQzYy41NTYuNzI1LS41NTUgMS41NTYtMS4xMDQuODI4eiIvPjwvZz48L3N2Zz4=" alt="openkakaotalk" />
                    <p> 오픈카카오톡 </p>
                    </a></li>
                </ul>

            </div>
        </div>
    )
}


export default QnA;