#pragma once
extern time_t timestep;
extern char clockstring[];
extern char secsstring[];
extern void init_clock();
extern void update_clock();
extern void start_clock();
extern void deinit_clock();