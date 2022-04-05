function wifi_cfg_save() {
    var baseHost = document.location.origin
    var ac = document.getElementById('wifi_accesspoint')
    var pwd = document.getElementById('wifi_passwd')
    var ac_value = encodeURIComponent(ac.value)
    var pwd_value = encodeURIComponent(pwd.value)
    var query = `${baseHost}/wifi_cfg?${ac.id}=${ac_value}&${pwd.id}=${pwd_value}`
    var notify_msg = document.getElementById('wifi_notify_msg')
    notify_msg.style.display='block';
    notify_msg.innerHTML = `${query}`;

    fetch(query)
        .then(response => {
	    console.log(`request to ${query} finished, status: ${response.status}`)
    })
}
