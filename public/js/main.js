//Sets sessionStorage sessionID to the UUID gotten from server
document.addEventListener("DOMContentLoaded", (e) => {
    sessionStorage.setItem("myID", document.getElementById("sessionID").innerText);
});
