import json
from django.http import HttpResponse, HttpResponseBadRequest, HttpRequest
from main.models import Device, Timeseries
from django.core.exceptions import ObjectDoesNotExist



def index(request):
    return HttpResponse("IoT web server is running. Waiting for gateway telemetry.")


def telemetry(request: HttpRequest):
    if request.method == 'POST':
        try:
            bad_request = False
            parsed_req = json.loads(request.body)

            gateway_name = parsed_req['name']
            try:
                gateway_id = Device.objects.get(name=gateway_name, is_gateway=True)
            except ObjectDoesNotExist:
                return HttpResponseBadRequest("Gateway {} not found.".format(gateway_name))

            sensor_data_list = parsed_req['data']

            for sensor_data in sensor_data_list:
                device_name = None
                timestamp = None
                for item in sensor_data.items():
                    if item[0] == 'name':
                        device_name = item[1]
                    elif item[0] == 'timestamp':
                        timestamp = item[1]
                    else:
                        try:
                            device_id = Device.objects.get(name=device_name, is_gateway=False)
                            ts = Timeseries(
                                device=device_id, key=item[0], value=item[1], timestamp=timestamp)
                            ts.save()
                        except ObjectDoesNotExist:
                            print("Device {} not found.".format(device_name))
                            bad_request = True

            if bad_request:
                return HttpResponseBadRequest("Something went wrong.")
            else:
                return HttpResponse()

        except json.JSONDecodeError:
            return HttpResponseBadRequest("Could not parse the json.")
