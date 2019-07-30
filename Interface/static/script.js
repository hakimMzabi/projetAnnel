$.ajaxSetup({
    beforeSend: function(xhr, settings) {
        if (settings.type === "POST" && !this.crossDomain) {
            xhr.setRequestHeader('X-CSRFToken', $('[name=csrfmiddlewaretoken]').val());
        }
    }
});


$('.upload').click(function(e) {
	$('#inputFile').trigger('click');
});

let startLoader = function(state = true){
	let self = $('.upload');

	if(state) {
		self.removeClass('do animateEnd');
		self.addClass('do');
	} else {
		self.addClass('animateEnd');
		$('#loadingText').html("<span style='color:#00b400'><i class='fa fa-check'></i></span>");
	}
};



let dropContainer = $("#drop-container");

dropContainer.on('dragenter', function(e) {
	e.preventDefault();
	$(this).css('border', '#39b311 2px dashed');
	$(this).css('background', '#f1ffef');
});

dropContainer.on('dragover', function(e) {
	e.preventDefault();
});

dropContainer.on('drop', function(e) {
	$(this).css('border', '#07c6f1 2px dashed');
	$(this).css('background', '#FFF');
	e.preventDefault();
	let files = e.originalEvent.dataTransfer.files;
	uploadFormData(files[0]);
});

$('input[type="file"]').change(function(){
	let image = this.files[0];

	let t = image.type;
	if(t === "image/gif" || t === "image/jpeg" || t === "image/png"){
		uploadFormData(image);
	}else{
		addNotification('error', 'Aïe !!', 'Type de fichier incorrect !');
	}

});

function getUrlVars() {
    let vars = {};
    let parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m,key,value) {
        vars[key] = value;
    });
    return vars;
}

function getUrlParam(parameter, defaultvalue){
    let urlparameter = defaultvalue;
    if(window.location.href.indexOf(parameter) > -1){
        urlparameter = getUrlVars()[parameter];
        }
    return urlparameter;
}

value = getUrlParam('model', 0);
console.log(value);
$("#modelName option[data-id='"+value+"']").attr('selected', 'selected');

function uploadFormData(image) {

	startLoader(true);

	$('#loadingText b').html(image.name);
	$('#loadingText').fadeIn();

	console.log(image);

	let formData = new FormData();
	formData.append('image', image);

	let modelName = $('#modelName').val();
	formData.append('modelName', modelName);

	$.ajax({
		url: "/upload",
		type: "POST",
		data: formData,
		contentType:false,
		cache: false,
		processData: false,
		success: function(response){

			if(response.classe){
				addNotification('success', 'Voilà !', "Drapeau : " + response.classe, false);
			}else{
				addNotification('success', 'Voilà !', "Erreur ! : " + response, false);
			}
			// addNotification('success', 'Voilà !', response.message);

			console.log(response);
			let reader = new FileReader();
			reader.onload = function (e) {
				$('#img-preview img').attr('src', e.target.result);
				$('.upload').hide();
				$('#img-preview').fadeIn();
				startLoader(false);
			};
			reader.readAsDataURL(image);
		},
		error: function(response){
			if (response.error) {
                addNotification('error', 'Ohh !', response.error);
            } else {
                addNotification('error', 'Erreur !!', "Une erreur serveur est survenue !");
            }
		}
	});
}

let addNotification = function(type, title, content, hideAfter = 2500){
	$.toast({
		heading: title,
		text: content,
		position: 'top-right',
		loaderBg: "#ff6849",
		icon: type,
		hideAfter: hideAfter,
		stack: 6,
	});
};