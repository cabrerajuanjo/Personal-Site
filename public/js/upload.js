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
    document.getElementById("get-processed-image-button").removeAttribute("disabled");
}

const disableUpload = () => {
    document.getElementById("get-processed-image-button").setAttribute('disabled', '');
}

//Display diferent options acording to selected filter
const displayOptions = () => {
    const filter = document.getElementById("filter").value;
    const optionContainer = document.getElementById("filter-option-container");
    optionContainer.innerHTML = "";
    if (filter === "mirror" || filter === "edges")
    {
        optionContainer.innerHTML += `
        <select id="filter_option" name="filter_option">
            <option value="0">Horizontal</option>
            <option value="1">Vertical</option>
            <option value="2" selected>Horizontal and vertical</option>
        </select>
        `;
    }else if(filter === "caricature")
    {
        optionContainer.innerHTML += `
        <input type="number" id="filter_option" name="filter_option" value="5" min="0" max="255">
        `
    }

    // uploadedImage = sessionStorage.getItem("uploaded_image_URL");
    // htmlContentToAppend = '<img src="' + uploadedImage + '" alt="">';
    // document.getElementById("input_img").innerHTML = htmlContentToAppend;
}

//Show image on browser
const showInputImage = (imageInput) => {
    const reader = new FileReader()
    reader.readAsDataURL(imageInput.files[0])
    reader.addEventListener("load", () => {
        uploadedImage = reader.result;
        sessionStorage.setItem("uploaded_image_URL", uploadedImage);
        htmlContentToAppend = '<img src="' + uploadedImage + '" alt="">';
        document.getElementById("input_img").innerHTML = htmlContentToAppend;
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
    document.getElementById("userID").value = sessionStorage.getItem("myID");
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
