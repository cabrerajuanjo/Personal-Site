//Checks if file signature matches the one given for a particular test (png, jpg, etc)
const bufferArrayArrayComparator =  (bufferBinaryArray, array) => {
    bufferSignature = bufferBinaryArray.slice(0, array.length);
    console.log(bufferSignature)
    if (bufferSignature.length != array.length)
    {
        return false
    }
    for (var i = 0; i < array.length; i++) {
        if (bufferSignature[i] != array[i]){
            return false;
        }
    }
    return true;
}

const enableUpload = () => {
    console.log("enableUpload")
    document.getElementById("upload_image_button").removeAttribute("disabled");
}

const disableUpload = () => {
    document.getElementById("upload_image_button").setAttribute('disabled', '');
}

//Show image on browser
const showInputImage = (imageInput) => {
    const reader = new FileReader()
    reader.readAsDataURL(imageInput.files[0])
    reader.addEventListener("load", () => {
        uploadedImage = reader.result
        htmlContentToAppend = '<img src="' + uploadedImage + '" alt="">'
        document.getElementById("input_img").innerHTML = htmlContentToAppend
    })
}

const preprocessFile = () => {
    //Checks for valid image type
    const imageInput = document.getElementById("image_file");
    const buffer = new FileReader();
    buffer.readAsArrayBuffer(imageInput.files[0]);

    buffer.addEventListener("load", () => {
        bufferBinaryArray = new Uint8Array(buffer.result);

        pngFileSignature = [ 137, 80, 78, 71, 13, 10, 26 ];
        jpgFileSignature = [255, 216, 255];
        ppmFileSignature = [80, 54];
        pgmFileSignature = [80, 50];

        if(bufferArrayArrayComparator(bufferBinaryArray, pngFileSignature))
        {
            console.log("Image is PNG");
            if(document.getElementById("show_preview").checked){
                showInputImage(imageInput);
            }
            enableUpload();

        }else if(bufferArrayArrayComparator(bufferBinaryArray, jpgFileSignature))
        {
            console.log("Image is JPG");
            if(document.getElementById("show_preview").checked){
                showInputImage(imageInput);
            }
            enableUpload();
        }else if(bufferArrayArrayComparator(bufferBinaryArray, ppmFileSignature))
        {
            console.log("Image is PPM");
            //showInputImage(imageInput);
            enableUpload();
        }else if(bufferArrayArrayComparator(bufferBinaryArray, pgmFileSignature))
        {
            console.log("Image is PGM");
            //showInputImage(imageInput);
            enableUpload();
        }else{
            disableUpload();
        }
        console.log("end");
    })


}

document.addEventListener("DOMContentLoaded", (e) => {
    disableUpload();
    document.getElementById("image_file").value = "";
    showPreview = document.getElementById("show_preview")
    showPreview.addEventListener("change", (e) => {
        if(!showPreview.checked)
        {
            document.getElementById("input_img").innerHTML = "";
        }else if(document.getElementById("image_file").files[0])
        {
            preprocessFile();
        }
    })
})
