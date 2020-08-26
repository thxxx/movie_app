import React from 'react';
import { HashRouter, Route } from "react-router-dom";
import { Apply, Explain, Navigation, QnA } from "./routes/inc.js";
import "./Landing.css"


function Landing() {
    return (
      <>
        <HashRouter>
    { /* <Navigation />  */ }
           <Apply />  `{/*exact는 /가 들어갔다고 다 Home으로 가지는걸 막기위해 */}`
           <Explain />
           <QnA />
        </HashRouter>
        <footer className="footer">
          <p id="reserved"> ®2019. SSIMPLAY All rights reserved. </p>
                  </footer>
      </>
    );
  
}



export default Landing;
