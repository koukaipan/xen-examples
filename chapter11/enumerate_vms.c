#include <stdio.h>

#include <curl/curl.h>
#include <xen/api/xen_all.h>

typedef struct
{
	xen_result_func func;
	void *handle;
} xen_comms;

static char *url;

static size_t
write_func(void *ptr, size_t size, size_t nmemb, xen_comms *comms)
{
	size_t n = size * nmemb;
	return comms->func(ptr, n, comms->handle) ? n : 0;
}


static int
call_func(const void *data, size_t len, void *user_handle,
		  void *result_handle, xen_result_func result_func)
{
	(void)user_handle;

	CURL *curl = curl_easy_init();
	if (!curl) {
		return -1;
	}

	xen_comms comms = {
		.func = result_func,
		.handle = result_handle
	};

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_func);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &comms);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);

	CURLcode result = curl_easy_perform(curl);

	curl_easy_cleanup(curl);

	return result;
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "Usage:\n\n%s <url> <username> <password>\n", argv[0]);
	}

	url = argv[1];

	/* General setup */
	xen_init();
	curl_global_init(CURL_GLOBAL_ALL);
	
	xen_session *session =
		xen_session_login_with_password(call_func, NULL, argv[2], argv[3]);

	if(session->ok)
	{
		/* Get the host */
		xen_host host;
		xen_session_get_this_host(session, &host, session);
		/* Get the set of VMs */
		struct xen_vm_set * VMs;
		xen_host_get_resident_vms(session, &VMs, host);
		/* Print the names */
		for(unsigned int i=0 ; i<VMs->size ; i++)
		{
			char * name;
			xen_host_get_name_label(session, &name, host);
			printf("VM %d: %s\n", i, name);
		}
	}
	else
	{
		printf(stderr, "Connection failed\n");
	}
	xen_session_logout(session);
	curl_global_cleanup();
	xen_fini();
	return 0;
}
