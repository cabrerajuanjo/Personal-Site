const showInputImage = () => {
    const imageInput = document.getElementById("image_file")
    var uploadedImage = ""
    const reader = new FileReader()
    reader.addEventListener("load", () => {
        uploadedImage = reader.result
        //document.getElementById("input_img").style.backgroundImage = 'url(' + uploadedImage + ')'
        htmlContentToAppend = '<img src="' + uploadedImage + '" alt="">'
        document.getElementById("input_img").innerHTML = htmlContentToAppend
    })
    reader.readAsDataURL(imageInput.files[0])
}

document.addEventListener("DOMContentLoaded", (e) => {

})
