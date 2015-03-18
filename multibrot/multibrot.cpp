#include <iostream>
#include <complex>
#include <queue>
#include <chrono>
#include <cmath>
#include <string>
#include <map>
#include <cstdlib> //system
#include <cstring> //stderr, wtf
#ifdef _WIN32
	#include <windows.h>

	#include <cerrno>
	//Wtf cerrno, you had one job
	//Also, I was just guessing some numbers
	#define EPROTO 44
	#define EOWNERDEAD 45
	
	#include <cstdio>
	#include <mingw.thread.h>
	#include <mutex>
	#include <mingw.mutex.h>
#else
	#include <thread>
	#include <mutex>
#endif
#include <png++/png.hpp>

std::string progressbar(unsigned w, double p)
{
	unsigned b = w*p;
	unsigned ps = p*100;
	std::string ret(w, ' ');
	ret[0]='[';
	for(unsigned i=1; i<w-1; i++)
		ret[i] = (i<b) ? '=' : (i==b) ? '>' : '-';
	ret[w-1]=']';
	
	if(ps>=100) ret[w/2-2] = '0' + (ps/100)%10;
	if(ps>=10)  ret[w/2-1] = '0' + (ps/10)%10;
	ret[w/2+0] = '0' + ps%10;
	ret[w/2+1] = '%';
	
	return ret;
}

struct thread_data_t 
{
	enum state_t 
	{
		inactive, 
		working, 
		waiting, 
		done
	};
	
	std::thread thread; //Namespaces ftw
	state_t state;
	double  progress;
};

struct task_t
{
	png::image<png::rgb_pixel>* image;
	
	unsigned maxiter;

	unsigned port_x, port_y;
	unsigned port_w, port_h;
	unsigned image_w, image_h;
	
	unsigned multisample;
	
	double view_x, view_y;
	double view_w, view_h;
};

std::queue<task_t> task_queue;
std::mutex task_lock;

template <typename T>
T lerp(T a, T b, T x){return (1.0-x)*a + x*b;}

png::rgb_pixel color_func(double i, unsigned max)
{
	double f = fmod(i/64.0, 1.0);
	return png::rgb_pixel(f*255, f*255, f*255);
}

void worker(thread_data_t& td)
{
	task_t task;
	while(1)
	{
		td.state = thread_data_t::state_t::waiting;
		task_lock.lock();
		if(task_queue.empty())
		{
			task_lock.unlock();
			td.state = thread_data_t::state_t::done;
			return;
		}
		
		task = task_queue.front();
		task_queue.pop();
		task_lock.unlock();
		
		td.state = thread_data_t::state_t::working;
		td.progress = 0.0;
		
		for(unsigned iy = task.port_y; iy < task.port_y + task.port_h; iy++)
		{
			for(unsigned ix = task.port_x; ix < task.port_x + task.port_w; ix++)
			{
				double rs, gs, bs;
				
				for(unsigned sy = 0; sy < task.multisample; sy++)
				{
					for(unsigned sx = 0; sx < task.multisample; sx++)
					{
						double u = (ix + double(sx)/task.multisample) / (double)task.image_w;
						double v = (iy + double(sy)/task.multisample) / (double)task.image_h;
						
						double x = task.view_x + u*task.view_w;
						double y = task.view_y + v*task.view_h;
						
						std::complex<double> c(x,y);
						std::complex<double> z(0,0);
						
						unsigned i;
						for(i=0; i<task.maxiter; i++)
						{
							z = z*z + c;
							if(std::abs(z) >= 2.0)
								break;
						}
						
						if(i == task.maxiter)
							i=0;
							
						png::rgb_pixel p = color_func(i, task.maxiter);
						rs += p.red; gs += p.green; bs += p.blue;
					}
				}
				
				rs /= task.multisample*task.multisample;
				gs /= task.multisample*task.multisample;
				bs /= task.multisample*task.multisample;
				
				task.image->set_pixel(ix, iy, png::rgb_pixel(rs,gs,bs));
				td.progress = double((iy-task.port_y)*task.port_w + ix-task.port_x)/(task.port_w*task.port_h);
			}
		}
		
		td.progress = 1.0;
	}
}

int main(int argc, char** argv)
{
	std::map<std::string, std::string> args;
	
	args["-vx"] = "0.0"; //view x
	args["-vy"] = "0.0"; //view y
	args["-vs"] = "1.0"; //view size
	
	args["-iw"] = "1920"; //image width 
	args["-ih"] = "1080"; //image height
	args["-ts"] = "512"; //tile size
	
	args["-mi"] = "512"; //maxiter 
	args["-ms"] = "1"; //multisample
	args["-t"] = "4"; //thread count
	
	args["-o"] = "mandelbrot.png"; //output
	
	for(unsigned i=1; i<argc; i++)
	{
		if(argv[i][0] == '-' && i+1<argc)
		{
			args.insert({argv[i], ""});
			args[argv[i]] = argv[i+1];
			i++;
		}
	}
	
	const unsigned thread_count = atoi(args["-t"].c_str());
	thread_data_t threads[thread_count];

	const unsigned image_width = atoi(args["-iw"].c_str());
	const unsigned image_height = atoi(args["-ih"].c_str());
	const double aspect = (double)image_width/image_height;
	
	const unsigned multisample = atoi(args["-ms"].c_str());
	
	const unsigned tile_width = atoi(args["-ts"].c_str());
	const unsigned tile_height = atoi(args["-ts"].c_str());
	
	double view_x = atof(args["-vx"].c_str());
	double view_y = atof(args["-vy"].c_str());
	double view_h = atof(args["-vs"].c_str());
	double view_w = view_h * aspect;
	
	const double view_xmin = view_x - view_w/2.0;
	const double view_ymin = view_y - view_h/2.0;
	const double view_xmax = view_x + view_w/2.0;
	const double view_ymax = view_y + view_h/2.0;
	
	const unsigned maxiter = atoi(args["-mi"].c_str());
	
	png::image<png::rgb_pixel> image(image_width, image_height);
	
	std::cout << "Filling task queue... ";
	for(unsigned y=0; y<image_height; y += tile_height)
	{
		for(unsigned x=0; x<image_width; x += tile_width)
		{
			task_t task;
			task.image = &image;
			task.multisample = multisample;
			
			task.maxiter = maxiter;
			task.port_x = x;
			task.port_y = y;
			
			task.port_w = std::min(image_width-x, tile_width);
			task.port_h = std::min(image_height-y, tile_height);
			
			task.image_w = image_width;
			task.image_h = image_height;
			
			task.view_x = view_xmin;
			task.view_y = view_ymin;
			task.view_w = view_w;
			task.view_h = view_h;
			
			task_queue.push(task);
		}
	}
	std::cout << task_queue.size() << " tasks" << std::endl;
	
	std::cout << "Starting threads... " << std::endl;
	for(unsigned i=0; i<thread_count; i++)
	{
		auto fn = [i, &threads]()->void{worker(threads[i]);};
		threads[i].thread = std::thread(fn);
	}
	std::cout << thread_count << " threads started" << std::endl;
	
	#ifdef _WIN32
		system("cls");
	#endif
	
	bool has_working_thread = 1;
	while(has_working_thread)
	{
		//TODO: Maybe lock?
		unsigned s = task_queue.size();
		
		has_working_thread = 0;
		
		#ifdef _WIN32
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
		#else 
			system("clear");
		#endif
		
		std::cout << "Remaining jobs: " << s << "\t\n";
		for(unsigned i=0; i<thread_count; i++)
		{
			std::cout << "Thread#" << i << "\t\n";
			std::cout << "    Status: ";
			switch(threads[i].state)
			{
				case thread_data_t::state_t::inactive: 
					std::cout << "inactive"; break;
				case thread_data_t::state_t::working: 
					std::cout << "working"; break;
				case thread_data_t::state_t::waiting: 
					std::cout << "waiting"; break;
				case thread_data_t::state_t::done: 
					std::cout << "done"; break;
			}
			std::cout << "\t\n";
			
			std::cout << "    Progress: " << progressbar(48, threads[i].progress) << "\t\n";
			std::cout << '\n';
			
			has_working_thread |= (threads[i].state != thread_data_t::state_t::done);
		}
		
		std::cout.flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	
	for(unsigned i=0; i<thread_count; i++)
	{
		std::cout << "Waiting for thread#" << i << "... ";
		threads[i].thread.join();
		std::cout << "done" << std::endl;
	}
	
	std::cout << "Saving file... ";
	image.write(args["-o"]);
	std::cout << "bye!" << std::endl;
		
	return 0;
}
