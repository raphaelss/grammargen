#!/usr/bin/env ruby

class Entry
  def initialize(rhs, prob)
    @sum = prob
    @clauses = {rhs => prob}
  end

  def single_clause?
    return @clauses.length == 1
  end

  def add_clause(rhs, prob)
    if @clauses.member?(rhs)
      @sum -= @clauses[rhs]
    end
    @sum += prob
    @clauses[rhs] = prob
  end

  def choose
    if single_clause?
      return @clauses.keys[0]
    end
    r = Random.rand(@sum)
    @clauses.each {|clause, prob|
      r -= prob
      if r <= 0
        return clause
      end
    }
  end

  def dump_c(out)
    if single_clause?
      out.puts("\t\t\tgen_seq(n-1, \"#{@clauses.keys[0]}\");")
    else
      out.puts("\t\t\tr = drand48() * #{@sum};")
      last = @clauses.length - 1
      cum_prob = 0
      @clauses.each_with_index { |rhs, prob, i|
        cum_prob += prob
        case i
	when 0
          out.puts("\t\t\tif (r < cum_prob) {")
	when last
          out.puts("\t\t\t} else if (r < cum_prob) {")
        else
          out.puts("\t\t\t} else {")
        end
        out.puts("\t\t\t\tgen_seq(n-1, \"#{rhs}\");")
      }
      out.puts("\t\t\t}")
    end
  end
end

class Grammar
  def initialize(axiom)
    @axiom = axiom
    @rules = {}
  end

  def add_rule(lhs, rhs, prob=1.0)
    if @rules.member?(lhs)
      @rules[lhs].add_clause(rhs, prob)
    else
      @rules[lhs] = Entry.new(rhs, prob)
    end
  end

  def gen_seq(n, str)
    if n > 0
      str.each_char { |c|
        if @rules.member?(c)
          gen_seq(n - 1, @rules[c].choose)
        else
          print c
        end
      }
    else
      print str
    end
  end

  def generate(n)
    gen_seq(n, @axiom)
  end

  def write_c_preamble(out)
   out.puts <<END_PREAMBLE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
gen_seq(int n, const char *str)
{
	if (n <= 0) {
		fwrite(str, 1, strlen(str), stdout);
		return;
	}
	double r;
	do {
		switch(*str) {
END_PREAMBLE
  end

  def write_c_postamble(out)
    out.puts <<END_POSTAMBLE
		default:
			putchar(*str);
			break;
		}
	} while (*++str);
}

int
main(int argc, char **argv)
{
	int n = atoi(argv[1]);
	int seed = atoi(argv[2]);
	srand48_deterministic(seed);
	gen_seq(n, "#{@axiom}");
	return 0;
}
END_POSTAMBLE
  end

  def dump_c(path)
    File.open(path, "w") { |out|
        write_c_preamble(out)
        @rules.each { |lhs, entry|
          out.puts("\t\tcase '#{lhs}':")
          entry.dump_c(out)
          out.puts("\t\t\tbreak;")
        }
	write_c_postamble(out)
    }
  end

  def self.read_clause(grammar, words)
    if words[0].length != 1
      puts("invalid clause")
      return nil
    end
    case words.length
    when 2
      grammar.add_rule(words[0], words[1])
    when 3
      prob = words[2].to_f
      if prob <= 0
        puts("invalid clause")
        return nil
      end
      grammar.add_rule(words[0], words[1], prob)
    else
    end
  end

  def self.from_file(path)
    grammar = nil
    File.foreach(path) { |line|
      words = line.split
      if words.empty?
        next
      end
      if grammar
	read_clause(grammar,words)
      elsif words.length == 1 && words[0].length == 1
        grammar = Grammar.new(words[0])
      else
        puts("axiom should have length 1")
      end
    }
    return grammar
  end
end

if __FILE__ == $PROGRAM_NAME
  grammar = Grammar.from_file(ARGV[0])
  grammar.generate(ARGV[1].to_i)
end
