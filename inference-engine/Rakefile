ALGS = [:bc, :fc, :tt]

task :iengine do
  puts 'Did you mean one of:'
  ALGS.each do |alg|
    puts " rake iengine:#{alg}"
  end
end

namespace :iengine do
  ALGS.each do |alg|
    task alg, [:filename] do |task, args|
      system("./bin/iengine #{alg.upcase} #{args.filename}")
    end
  end
end
