const displayOptions = () => {
    const filter = document.getElementById("filter").value;
    const optionContainer = document.getElementById("filter-option-container");
    optionContainer.innerHTML = "";
    if (filter === "mirror" || filter === "edges")
    {
        optionContainer.innerHTML += `
        <select id="filter-option" name="filter-option">
          <option value="0">Vertical</option>
          <option value="1">Horizontal</option>
          <option value="2" selected>Horizontal and vertical</option>
        </select>
        `;
    }else if(filter === "caricature")
    {
        optionContainer.innerHTML += `
        <input type="number" id="filter-option" name="filter-option" value="5" min="0" max="255">
        `
    }

    // uploadedImage = sessionStorage.getItem("uploaded_image_URL");
    // htmlContentToAppend = '<img src="' + uploadedImage + '" alt="">';
    // document.getElementById("input_img").innerHTML = htmlContentToAppend;
}

const getProcessedImage = () => {
    const filterType = getElementById("filter").value;
    const filterOption = getElementById("filter-option").value;
    fetch("/set-filter?filter=" + filterType + "&filter-option=" + filterOption).then(response)
}
