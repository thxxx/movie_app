import React from "react";
import {Link } from "react-router-dom"
import "./Navigations.css";

function Navigation(){
    return <span className="nav">
        <a href="#main" className="pageBtn">Home</a>
        <a href="#explain" className="pageBtn"> 서비스 소개 </a>
        <a href="#qna" className="pageBtn"> 문의사항 </a>
    </span>
}

export default Navigation;
