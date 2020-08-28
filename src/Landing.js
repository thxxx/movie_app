import React from 'react';
import { HashRouter, Route } from "react-router-dom";
import { Apply, Explain, Navigation, QnA } from "./routes/inc.js";
import "./Landing.css"


function Landing() {
    return (
      <>
    
      <Navigation />
      {/*Navigation*/}
      <nav class="navbar navbar-expand-lg navbar-light fixed-top py-3" id="mainNav">
            <div class="container">
                <a class="navbar-brand js-scroll-trigger" href="#page-top">Start Bootstrap</a>
                <button class="navbar-toggler navbar-toggler-right" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation"><span class="navbar-toggler-icon"></span></button>
                <div class="collapse navbar-collapse" id="navbarResponsive">
                    <ul class="navbar-nav ml-auto my-2 my-lg-0">
                        <li class="nav-item"><a class="nav-link js-scroll-trigger" href="#about">About</a></li>
                        <li class="nav-item"><a class="nav-link js-scroll-trigger" href="#services">Services</a></li>
                        <li class="nav-item"><a class="nav-link js-scroll-trigger" href="#portfolio">Portfolio</a></li>
                        <li class="nav-item"><a class="nav-link js-scroll-trigger" href="#contact">Contact</a></li>
                    </ul>
                </div>
            </div>
        </nav>
        
        <HashRouter>
    { /* <Navigation />  */ }
           <Apply />  `{/*exact는 /가 들어갔다고 다 Home으로 가지는걸 막기위해 */}`
           <Explain />
           <QnA />
        </HashRouter>
        {/*Footer*/}
        <footer class="bg-light py-4">
            <div class="container"><div class="small text-center text-muted">Copyright © 2020 - Befriends Group <br/> All Rights Reserved </div></div>
        </footer>
      </>
    );
  
}



export default Landing;
