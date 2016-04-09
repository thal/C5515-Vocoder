close all;

Fs = 8000;
FsBy2 = Fs/2;

% Same passband ripple and stopband attenuation for all filters
Rp = 0.2;       % Maximum passband ripple
Rs = 60;        % Minimum stop band attenuation
N = 30;

%centers = [240, 360, 480, 600, 720, 840, 1000, 1150, 1300, 1450, 1600];
%widths = [120, 120, 120, 120, 120, 120, 150, 150, 150, 150, 150];
centers = [ 300, 600, 1200, 2400];
widths = [300, 600, 1200, 2400];
filts = zeros(length(centers), N+1);
% Band Pass 1
for i = 1:length(centers)
    fcenter = centers(i);
    fwidth = widths(i);
    Wp = [fcenter-fwidth/2, fcenter+fwidth/2]/FsBy2;
    bpf = fir1(N, Wp, 'bandpass');
    filts(i, 1:N+1) = bpf;
    [H1, W1] = freqz(bpf);
    
%     plot(W1*FsBy2/pi, 20*log10(abs(H1)),'b')
%     axis([0 3300 -100, 1]);
%     xlabel('Frequency(Hz)');
%     ylabel('Magnitude(dB)');
%     figure();
end

bpf1 = round(filts(1, 1:N+1) .* 32767);
bpf2 = round(filts(2, 1:N+1) .* 32767);
bpf3 = round(filts(3, 1:N+1) .* 32767);
bpf4 = round(filts(4, 1:N+1) .* 32767);

