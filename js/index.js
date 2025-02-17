function setHTML(htmlAddres)
{
    const frame = document.getElementById("page-frame");
    frame.setAttribute("src", "html/"+ htmlAddres +".html");
}