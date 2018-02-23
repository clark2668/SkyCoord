from astropy.coordinates import EarthLocation, SkyCoord, AltAz
from astropy.time import Time
from astropy import units as u
from astropy.coordinates import AltAz

def main():

	A2_location = EarthLocation(lon='-109.70392',lat='-89.8105588',height=2649.85*u.m)
	A3_location = EarthLocation(lon='-88.086572',lat='-89.81832715',height=2650.77*u.m)
	
	time = Time('2013-01-04 17:18:07')
	
	A2_frame = AltAz(location=A2_location, obstime=time)
	A3_frame = AltAz(location=A3_location, obstime=time)
	
	coord = SkyCoord(ra=174.09*u.degree, dec=25.92*u.degree)
	coordA2 = coord.transform_to(A2_frame)
	coordA3 = coord.transform_to(A3_frame)
	
	print coordA2
	print coordA3
	
main()
