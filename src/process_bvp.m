% function process_bvp()
directory_path = '/Users/Ahmed/Dropbox/College/Senior/Thesis/sdms-engine/src/tests/test07/BVP/';

bvp_files = dir(directory_path);
for i = 1:1:length(bvp_files)
    name = string(bvp_files(i).name);
    if startsWith(name, 'BVP_data_phase')
        filename = char(directory_path + name);
        data = csvread(filename);
        times = data(:,1);
        bvp = data(:,2);

        % Scale time to start from zero
        start_time = times(1);
        for j = 1:1:length(times)
            times(j) = times(j) - start_time;
        end

        [s, ps] = spectrogram(bvp);
        % conjugate transpose of s
        c_transpose = ctranspose(s);
        real_matrix = s * c_transpose;
        
% %         real_matrix = mtimes(c_transpose, s');
        
        spectrogram(bvp,'yaxis')
        
        negative_bvp = -1 * bvp;
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
        filtered_bvp_negative = filtfilt(lpFilter,1, negative_bvp); 
        
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
        
        % Range normalize normal negative bvp
        normalized_bvp_normal_negative = zeros(length(negative_bvp), 1);
        min_bvp = min(negative_bvp);
        max_bvp = max(negative_bvp);
        for j = 1:1:length(negative_bvp)
            normalized_bvp_normal_negative(j) = (negative_bvp(j) - min_bvp)/(max_bvp - min_bvp);
        end

        % Range normalize filtered bvp
        normalized_bvp_filtered = zeros(length(bvp), 1);
        min_bvp = min(filtered_bvp);
        max_bvp = max(filtered_bvp);
        for j = 1:1:length(filtered_bvp)
            normalized_bvp_filtered(j) = (filtered_bvp(j) - min_bvp)/(max_bvp - min_bvp);
        end
    
        % Range normalize filtered negative bvp
        normalized_bvp_filtered_negative = zeros(length(negative_bvp), 1);
        min_bvp = min(filtered_bvp_negative);
        max_bvp = max(filtered_bvp_negative);
        for j = 1:1:length(filtered_bvp_negative)
            normalized_bvp_filtered_negative(j) = (filtered_bvp_negative(j) - min_bvp)/(max_bvp - min_bvp);
        end

        % Find peaks in bvp
        [pks, indicies] = findpeaks(normalized_bvp_filtered, 'MinPeakHeight',0.1, 'MinPeakDistance',32);
        peak_times = zeros(length(indicies), 1);
        for j = 1:1:length(indicies)
            peak_times(j) = times(indicies(j));
        end

        % Find peaks in negative bvp
        [pks_negative, indicies_negative] = findpeaks(normalized_bvp_filtered_negative, 'MinPeakHeight',0.1, 'MinPeakDistance',32);
        peak_times_negative = zeros(length(indicies_negative), 1);
        for j = 1:1:length(indicies_negative)
            peak_times_negative(j) = times(indicies_negative(j));
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
        ibi_negative = zeros(length(indicies_negative) - 1, 2);
        mags = zeros(length(indicies) - 1, 2);
        mags_negative = zeros(length(indicies_negative) - 1, 2);
        j = 1;
        for k = 1:1:length(peak_times) - 1
            prev_time = peak_times(k);
            prev_mag = normalized_bvp_filtered(indicies(k));
            time = peak_times(k + 1);
            mag = normalized_bvp_filtered(indicies(k + 1));
            ibi(k,1) = time;
            mags(k,1) = time;
            ibi(k,2) = time - prev_time;
            mags(k,2) = abs(mag - prev_mag);
        end
        
        j = 1;
        for k = 1:1:length(peak_times_negative) - 1
            prev_time = peak_times_negative(k);
            prev_mag = normalized_bvp_filtered_negative(indicies_negative(k));
            time = peak_times_negative(k + 1);
            mag = normalized_bvp_filtered_negative(indicies_negative(k + 1));
            ibi_negative(k,1) = time;
            mags_negative(k,1) = time;
            ibi_negative(k,2) = time - prev_time;
            mags_negative(k,2) = abs(mag - prev_mag);
        end
        
        path = string(directory_path(1:length(directory_path)-4)) + string('IBI/IBI_data_phase');
        name = char(name);
        num = char(name(15:length(name) - 4));
        output_filename = char(path + num + '.txt');
        ibi = [ibi mags(:,2)];
        dlmwrite(output_filename, ibi,' '); 

        path = string(directory_path(1:length(directory_path)-4)) + string('IBI/NIBI_data_phase');
        name = char(name);
        num = char(name(15:length(name) - 4));
        output_filename_negative = char(path + num + '.txt');
        ibi_negative = [ibi_negative mags_negative(:,2)];
        dlmwrite(output_filename_negative, ibi_negative,' ');
    end
end
% clear;