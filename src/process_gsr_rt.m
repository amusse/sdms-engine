function process_gsr_rt()
directory_path = '/Users/Ahmed/Dropbox/College/Senior/Thesis/sdms-engine/data/';

gsr_files = dir(directory_path);
for i = 1:1:length(gsr_files)
    name = string(gsr_files(i).name);
    if startsWith(name, 'GSR_data_')
        filename = char(directory_path + name);
        data = dlmread(filename, ' ');
        
        times = data(:,1);
        gsr = data(:,2);
        
        % Scale times to start from 0
        start_time = times(1);
        for j = 1:1:length(times)
            times(j) = times(j) - start_time;
        end
        
        % Range normalize
        normalized_gsr = zeros(length(gsr), 1);
        min_gsr = min(gsr);
        max_gsr = max(gsr);
        for j = 1:1:length(gsr)
            normalized_gsr(j) = (gsr(j) - min_gsr)/(max_gsr - min_gsr);
        end
        
        % Add event markers
        event_markers = zeros(length(gsr), 1);
        new_data = [times, normalized_gsr, event_markers];
        step = 10*4;
        for j = 1:step:length(new_data)
            new_data(j,3) = 99;
        end
        
        % Extract features
        output_filename = [filename(1:length(filename)-4), '_marked.txt'];
        dlmwrite(output_filename, new_data,' ');
        
        Ledalab(output_filename, 'open', 'text', 'analyze','CDA', 'export_era', [0 30 .01 2]);
        era_filename = [output_filename(1:length(output_filename)-4), '_era.txt'];
        cda_filename = [filename(1:length(filename)-4), '_cda.txt'];
        movefile(era_filename, cda_filename);
        
%         Ledalab(output_filename, 'open', 'text', 'analyze','DDA', 'export_era', [0 30 .01 2]);
%         dda_filename = [filename(1:length(filename)-4), '_dda.txt'];
%         movefile(era_filename, dda_filename);
        
        % Clean up
        mat_file_name = [output_filename(1:length(output_filename) - 3), 'mat'];
        delete(output_filename);
        delete(mat_file_name);
    end
    delete('batchmode_protocol.mat');

end
clear;
