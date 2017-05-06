% function process_bvp_rt()
directory_path = '/Users/Ahmed/Dropbox/College/Senior/Thesis/sdms-engine/data/';

bvp_files = dir(directory_path);
for i = 1:1:length(bvp_files)
    name = string(bvp_files(i).name);
    if startsWith(name, 'BVP_data_')
        filename = char(directory_path + name);
        data = csvread(filename);
        times = data(:,1);
        bvp = data(:,2);

        % Scale time to start from zero
        start_time = times(1);
        for j = 1:1:length(times)
            times(j) = times(j) - start_time;
        end

           
        % Frequency response before filter
        NFFT = length(bvp);
        Y = fft(bvp,NFFT);
        F = ((0:1/NFFT:1-1/NFFT)*64).';
        magnitudeY = abs(Y);        % Magnitude of the FFT
        phaseY = unwrap(angle(Y));  % Phase of the FFT
%             helperFrequencyAnalysisPlot1(F,magnitudeY,phaseY,NFFT);

        % Create low-pass filter with 10 Hz cut-off
        lpFilter = fir1(50,10/32,'low');
        % freqz(lpFilter,1)

        % Apply low-pass filter on data
        filtered_bvp = filtfilt(lpFilter,1, bvp); 

        % Frequency response after filter
        NFFT = length(filtered_bvp);
        Y = fft(filtered_bvp,NFFT);
        F = ((0:1/NFFT:1-1/NFFT)*64).';
        magnitudeY = abs(Y);        % Magnitude of the FFT
        phaseY = unwrap(angle(Y));  % Phase of the FFT
%             helperFrequencyAnalysisPlot1(F,magnitudeY,phaseY,NFFT);

        % Range normalize normal bvp
        normalized_bvp_normal = zeros(length(bvp), 1);
        min_bvp = min(bvp);
        max_bvp = max(bvp);
        for j = 1:1:length(bvp)
            normalized_bvp_normal(j) = (bvp(j) - min_bvp)/(max_bvp - min_bvp);
        end

        % Range normalize filtered bvp
        normalized_bvp_filtered = zeros(length(bvp), 1);
        min_bvp = min(filtered_bvp);
        max_bvp = max(filtered_bvp);
        for j = 1:1:length(filtered_bvp)
            normalized_bvp_filtered(j) = (filtered_bvp(j) - min_bvp)/(max_bvp - min_bvp);
        end


        % Find peaks in bvp
        [pks, indicies] = findpeaks(normalized_bvp_filtered, 'MinPeakHeight',0.1, 'MinPeakDistance',32);
        peak_times = zeros(length(indicies), 1);
        for j = 1:1:length(indicies)
            peak_times(j) = times(indicies(j));
        end

        % BVP before and filter
%         figure
%         subplot(2,1,1);
%         plot(times, normalized_bvp_normal);
%         title('Unfiltered BVP')
%         xlabel('Time (s)')
%         ylabel('Normalized BVP')
% 
%         subplot(2,1,2); 
%         hold on
%         plot(times, normalized_bvp_filtered, ''); 
%         plot(peak_times,pks,'rs','MarkerFaceColor','y');
%         grid on
%         ylabel('Normalized BVP')
%         title('Filtered BVP')
%         xlabel('Time (s)')  

        % Calculate IBI as difference in time between peak times
        ibi = zeros(length(indicies) - 1, 2);
        j = 1;
        for k = 1:1:length(peak_times) - 1
            prev_time = peak_times(k);
            time = peak_times(k + 1);
            ibi(k,1) = time;
            ibi(k,2) = time - prev_time;
        end
        
        
        name = char(name);
        fn = ['IBI',name(4:length(name)-4), '_real.txt'];
        output_filename = [directory_path, fn];
        dlmwrite(output_filename, ibi,' ');
    end
end
clear;
